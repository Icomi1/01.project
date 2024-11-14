#include "GPS-BD.h" 
#include "delay.h" 								   								   
#include "stdio.h"	 
#include "stdarg.h"	 
#include "string.h"	 
#include "math.h"
#include "usart.h"		
#include "oled.h"
////////////////////////////////////////////////////////////////////////////////// 	   
GPS_Data User_GPS_Data;
//从buf里面得到第cx个逗号所在的位置
//返回值:0~0XFE,代表逗号所在位置的偏移.
//       0XFF,代表不存在第cx个逗号			
__align(4) u8 dtbuf[50];   								//打印缓存器	
const u8*fixmode_tbl[4]={"Fail","Fail"," 2D "," 3D "};	//fix mode字符串 
nmea_msg gpsx; 
u8 NMEA_Comma_Pos(u8 *buf,u8 cx)
{	 		    
	u8 *p=buf;
	
	while(cx)
	{		 
		if(*buf=='*'||*buf<' '||*buf>'z')return 0XFF;//遇到'*'或者非法字符,则不存在第cx个逗号
		if(*buf==',')cx--;
		buf++;
	}
	return buf-p;	 
}
//m^n函数
//返回值:m^n次方.
u32 NMEA_Pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}
//str转换为数字,以','或者'*'结束
//buf:数字存储区
//dx:小数点位数,返回给调用函数
//返回值:转换后的数值
int NMEA_Str2num(u8 *buf,u8*dx)
{
	u8 *p=buf;
	u32 ires=0,fres=0;
	u8 ilen=0,flen=0,i;
	u8 mask=0;
	int res;
	while(1) //得到整数和小数的长度
	{
		if(*p=='-'){mask|=0X02;p++;}//是负数
		if(*p==','||(*p=='*'))break;//遇到结束了
		if(*p=='.'){mask|=0X01;p++;}//遇到小数点了
		else if(*p>'9'||(*p<'0'))	//有非法字符
		{	
			ilen=0;
			flen=0;
			break;
		}	
		if(mask&0X01)flen++;
		else ilen++;
		p++;
	}
	if(mask&0X02)buf++;	//去掉负号
	for(i=0;i<ilen;i++)	//得到整数部分数据
	{  
		ires+=NMEA_Pow(10,ilen-1-i)*(buf[i]-'0');
	}
	if(flen>5)flen=5;	//最多取5位小数
	*dx=flen;	 		//小数点位数
	for(i=0;i<flen;i++)	//得到小数部分数据
	{  
		fres+=NMEA_Pow(10,flen-1-i)*(buf[ilen+1+i]-'0');
	} 
	res=ires*NMEA_Pow(10,flen)+fres;
	if(mask&0X02)res=-res;		   
	return res;
}	  							 
//分析GPGSV信息
//gpsx:nmea信息结构体
//buf:接收到的GPS数据缓冲区首地址
void NMEA_GPGSV_Analysis(nmea_msg *gpsx,u8 *buf)
{
	u8 *p,*p1,dx;
	u8 len,i,j,slx=0;
	u8 posx;   	 
	p=buf;
	p1=(u8*)strstr((const char *)p,"$GNGSV");
	len=p1[7]-'0';								//得到GPGSV的条数
	posx=NMEA_Comma_Pos(p1,3); 					//得到可见卫星总数
	if(posx!=0XFF)gpsx->svnum=NMEA_Str2num(p1+posx,&dx);
	for(i=0;i<len;i++)
	{	 
		p1=(u8*)strstr((const char *)p,"$GNGSV");  
		for(j=0;j<4;j++)
		{	  
			posx=NMEA_Comma_Pos(p1,4+j*4);
			if(posx!=0XFF)gpsx->slmsg[slx].num=NMEA_Str2num(p1+posx,&dx);	//得到卫星编号
			else break; 
			posx=NMEA_Comma_Pos(p1,5+j*4);
			if(posx!=0XFF)gpsx->slmsg[slx].eledeg=NMEA_Str2num(p1+posx,&dx);//得到卫星仰角 
			else break;
			posx=NMEA_Comma_Pos(p1,6+j*4);
			if(posx!=0XFF)gpsx->slmsg[slx].azideg=NMEA_Str2num(p1+posx,&dx);//得到卫星方位角
			else break; 
			posx=NMEA_Comma_Pos(p1,7+j*4);
			if(posx!=0XFF)gpsx->slmsg[slx].sn=NMEA_Str2num(p1+posx,&dx);	//得到卫星信噪比
			else break;
			slx++;	   
		}   
 		p=p1+1;//切换到下一个GPGSV信息
	}   
}
//分析GPGGA信息
//gpsx:nmea信息结构体
//buf:接收到的GPS数据缓冲区首地址
void NMEA_GPGGA_Analysis(nmea_msg *gpsx,u8 *buf)
{
	u8 *p1,dx;			 
	u8 posx;    
	p1=(u8*)strstr((const char *)buf,"$GNGGA");
	posx=NMEA_Comma_Pos(p1,6);								//得到GPS状态
	if(posx!=0XFF)gpsx->gpssta=NMEA_Str2num(p1+posx,&dx);	
	posx=NMEA_Comma_Pos(p1,7);								//得到用于定位的卫星数
	if(posx!=0XFF)gpsx->posslnum=NMEA_Str2num(p1+posx,&dx); 
	posx=NMEA_Comma_Pos(p1,9);								//得到海拔高度
	if(posx!=0XFF)gpsx->altitude=NMEA_Str2num(p1+posx,&dx);  
}
//分析GPGSA信息
//gpsx:nmea信息结构体
//buf:接收到的GPS数据缓冲区首地址
void NMEA_GPGSA_Analysis(nmea_msg *gpsx,u8 *buf)
{
	u8 *p1,dx;			 
	u8 posx; 
	//u8 i;   
	p1=(u8*)strstr((const char *)buf,"$GNGSA");
	posx=NMEA_Comma_Pos(p1,2);								//得到定位类型
	if(posx!=0XFF)gpsx->fixmode=NMEA_Str2num(p1+posx,&dx);	
}
//分析GPRMC信息
//gpsx:nmea信息结构体
//buf:接收到的GPS数据缓冲区首地址
//返回值：0获取到1次时间；1失败
u8 NMEA_GPRMC_Analysis(nmea_msg *gpsx,u8 *buf)
{
	u8 *p1,dx;			 
	u8 posx;     
	u32 temp;	   
	//u8 gps_date_time[7];
	u8 sta_get_date_time=0;	//0--未获取，1-只获取到time,2-只获取到DATE，3-或获取到全部
	float rs;  
	p1=(u8*)strstr((const char *)buf,"RMC");//"$GPRMC",经常有&和GPRMC分开的情况,故只判断GPRMC.
	posx=NMEA_Comma_Pos(p1,1);								//得到UTC时间
	if(posx!=0XFF)
	{
		NMEA_Str2num(p1+posx,&dx)/NMEA_Pow(10,dx);	 	//得到UTC时间,去掉ms	
		gpsx->utc.hour=(*(p1+posx+0)-0x30)*10+(*(p1+posx+1)-0x30);
		gpsx->utc.min=(*(p1+posx+2)-0x30)*10+(*(p1+posx+3)-0x30);
		gpsx->utc.sec=(*(p1+posx+4)-0x30)*10+(*(p1+posx+5)-0x30); 
		sta_get_date_time=1;		
	}	
	posx=NMEA_Comma_Pos(p1,9);								//得到UTC日期
	if(posx!=0XFF)
	{
		NMEA_Str2num(p1+posx,&dx);		 				//得到UTC日期
		gpsx->utc.date=(*(p1+posx+0)-0x30)*10+(*(p1+posx+1)-0x30);
		gpsx->utc.month=(*(p1+posx+2)-0x30)*10+(*(p1+posx+3)-0x30);
	  gpsx->utc.year=(*(p1+posx+4)-0x30)*10+(*(p1+posx+5)-0x30);			
		if(sta_get_date_time==1)	sta_get_date_time=3;
		else sta_get_date_time=2;
	}
	posx=NMEA_Comma_Pos(p1,3);								//得到纬度
	if(posx!=0XFF)
	{
		temp=NMEA_Str2num(p1+posx,&dx);		 	 
		gpsx->latitude=temp/NMEA_Pow(10,dx+2);	//得到°
		rs=temp%NMEA_Pow(10,dx+2);				//得到'		 
		gpsx->latitude=gpsx->latitude*NMEA_Pow(10,5)+(rs*NMEA_Pow(10,5-dx))/60;//转换为° 
	
	}
	posx=NMEA_Comma_Pos(p1,4);								//南纬还是北纬 
	if(posx!=0XFF)	User_GPS_Data.nshemi=gpsx->nshemi=*(p1+posx);					 			
 	posx=NMEA_Comma_Pos(p1,5);								//得到经度
	if(posx!=0XFF)
	{												  
		temp=NMEA_Str2num(p1+posx,&dx);		 	 
		gpsx->longitude=temp/NMEA_Pow(10,dx+2);	//得到°
		rs=temp%NMEA_Pow(10,dx+2);				//得到'		 
		gpsx->longitude=gpsx->longitude*NMEA_Pow(10,5)+(rs*NMEA_Pow(10,5-dx))/60;//转换为° 
		
	}
	posx=NMEA_Comma_Pos(p1,6);								//东经还是西经
	if(posx!=0XFF)	User_GPS_Data.ewhemi=gpsx->ewhemi=*(p1+posx);		 	
	posx=NMEA_Comma_Pos(p1,2);								//得到是否定位状态
	if(posx!=0XFF)
	{	
		if(*(p1+posx)=='A')	
		{
			User_GPS_Data.fix_mode=1;	
			User_GPS_Data.latitude=gpsx->latitude;
			User_GPS_Data.longitude=gpsx->longitude;
			gpsx->fixmode=2;//定位			
		}
		else 
		{			
			User_GPS_Data.fix_mode=0;
			User_GPS_Data.longitude=User_GPS_Data.latitude=0;
			gpsx->fixmode=0;		
		}
		if((sta_get_date_time==3)&&(gpsx->fixmode==2))	return 0;	//获取到日期和时间并且已经定位
		else return 1;
	}	
	return 1;
}
//分析GPVTG信息
//gpsx:nmea信息结构体
//buf:接收到的GPS数据缓冲区首地址
void NMEA_GPVTG_Analysis(nmea_msg *gpsx,u8 *buf)
{
	u8 *p1,dx;			 
	u8 posx;    
	p1=(u8*)strstr((const char *)buf,"$GPVTG");							 
	posx=NMEA_Comma_Pos(p1,7);								//得到地面速率
	if(posx!=0XFF)
	{
		gpsx->speed=NMEA_Str2num(p1+posx,&dx);
		if(dx<3)gpsx->speed*=NMEA_Pow(10,3-dx);	 	 		//确保扩大1000倍
	}
}  
//提取NMEA-0183信息
//gpsx:nmea信息结构体
//buf:接收到的GPS数据缓冲区首地址
u8 GPS_Analysis(nmea_msg *gpsx,u8 *buf)
{
	NMEA_GPGSV_Analysis(gpsx,buf);	//GPGSV解析
	NMEA_GPGGA_Analysis(gpsx,buf);	//GPGGA解析 	
	NMEA_GPGSA_Analysis(gpsx,buf);	//GPGSA解析
	if(NMEA_GPRMC_Analysis(gpsx,buf))	
		;//return 1;	//获取失败
	//else 
	return 0;
//	NMEA_GPVTG_Analysis(gpsx,buf);	//GPVTG解析
}

//GPS校验和计算
//buf:数据缓存区首地址
//len:数据长度
//cka,ckb:两个校验结果.
void Ublox_CheckSum(u8 *buf,u16 len,u8* cka,u8*ckb)
{
	u16 i;
	*cka=0;*ckb=0;
	for(i=0;i<len;i++)
	{
		*cka=*cka+buf[i];
		*ckb=*ckb+*cka;
	}
}
//入口是10进制时间，出口是BCD码时间
void UTC2BTC(nmea_utc_time *GPS)
{
	GPS->sec++;
	if(GPS->sec>59)
	{
		GPS->sec=0;
		GPS->min++;
		if(GPS->min>59)
		{
			GPS->min=0;
			GPS->hour++;
		}
	}
	GPS->hour+=8;
	/*???????????8???,???8???*/
	if(GPS->hour>23)
	{
		GPS->hour=0;
		GPS->date+=1;
	}
	if(GPS->month==2)	/*????*/
	{
		if(GPS->year%100==0)		
		{
			if(GPS->year%400==0)
			{
				if(GPS->date>29)
				{
					GPS->date=1;
					GPS->month++;
				}
			}
			else
			{
				if(GPS->date>28)
				{
					GPS->date=1;
					GPS->month++;
				}
			}			
		}
		else
		{
			if(GPS->year%4==0)
			{
				if(GPS->date>29)
				{
					GPS->date=1;
					GPS->month++;
				}
			}
			else
			{
				if(GPS->date>28)
				{
					GPS->date=1;
				  GPS->month++;
			  }
			}
		}
	}
	else if(GPS->month==4||GPS->month==6||GPS->month==9||GPS->month==11)
	{
		if(GPS->date>30)
		{
			GPS->date=1;
			GPS->month++;
		}
	}
	else if(GPS->month==1||GPS->month==3||GPS->month==5||GPS->month==7||GPS->month==8||GPS->month==10||GPS->month==12)
	{
		if(GPS->date>31)
		{
			GPS->date=1;
			GPS->month++;
		}
	}	
	if(GPS->month>12)
	{
		GPS->month=1;
		GPS->year++;
	}
	GPS->year=(GPS->year/10)*16+(GPS->year%10);
	GPS->month=(GPS->month/10)*16+(GPS->month%10);
	GPS->date=(GPS->date/10)*16+(GPS->date%10);
	GPS->hour=(GPS->hour/10)*16+(GPS->hour%10);
	GPS->min=(GPS->min/10)*16+(GPS->min%10);
	GPS->sec=(GPS->sec/10)*16+(GPS->sec%10);
}

//显示GPS定位信息 
void Gps_Msg_Show(void)
{
 	float tp;		   
	//POINT_COLOR=BLUE;  	 
	tp=gpsx.longitude;	   
	sprintf((char *)dtbuf,"Lon:%.5f %1c   ",tp/=100000,gpsx.ewhemi);	//得到经度字符串
 //	OLED_ShowString(0,0,dtbuf);
	//LCD_ShowString(30,130,200,16,16,dtbuf);	 	   
	tp=gpsx.latitude;	   
	sprintf((char *)dtbuf,"Lat:%.5f %1c   ",tp/=100000,gpsx.nshemi);	//得到纬度字符串
 //	OLED_ShowString(0,2,dtbuf);
	//LCD_ShowString(30,150,200,16,16,dtbuf);	 	 
	tp=gpsx.altitude;	   
 	sprintf((char *)dtbuf,"Altitude:%.1fm     ",tp/=10);	    			//得到高度字符串
 	//LCD_ShowString(30,170,200,16,16,dtbuf);	 			   
	tp=gpsx.speed;	   
 	sprintf((char *)dtbuf,"Speed:%.3fkm/h     ",tp/=1000);		    		//得到速度字符串	 
 	//LCD_ShowString(30,190,200,16,16,dtbuf);	 				    
	if(gpsx.fixmode<=3)														//定位状态
	{  
		sprintf((char *)dtbuf,"Fix Mode:%s",fixmode_tbl[gpsx.fixmode]);	
	  //LCD_ShowString(30,210,200,16,16,dtbuf);			   
	}	 	   
	//OLED_ShowString(0,4,dtbuf);
	sprintf((char *)dtbuf,"Valid satellite:%02d",gpsx.posslnum);	 		//用于定位的卫星数
 	//LCD_ShowString(30,230,200,16,16,dtbuf);	    
	sprintf((char *)dtbuf,"Visible satellite:%02d",gpsx.svnum%100);	 		//可见卫星数
 	//LCD_ShowString(30,250,200,16,16,dtbuf);		 
	sprintf((char *)dtbuf,"UDate:%02d/%02d/%02d",gpsx.utc.year,gpsx.utc.month,gpsx.utc.date);	//显示UTC日期
	//OLED_ShowString(0,4,dtbuf);
	//printf("year2:%d\r\n",gpsx.utc.year);
	//LCD_ShowString(30,270,200,16,16,dtbuf);		    
	sprintf((char *)dtbuf,"UTime:%02d:%02d:%02d",gpsx.utc.hour,gpsx.utc.min,gpsx.utc.sec);	//显示UTC时间
  //OLED_ShowString(0,6,dtbuf);
	//LCD_ShowString(30,290,200,16,16,dtbuf);		  
}	 

void GPS_task(void )
{	
	u16 i,rxlen;
//	u8 sys_date[4],sys_time[4];
	u8 USART1_TX_BUF[USART3_MAX_RECV_LEN]; 					//串口1,发送缓存区
  //nmea_msg gpsx;	
	if(USART_RX_STA&0X8000)		//接收到一次数据了
	//if(1)
	{
		rxlen=USART_RX_STA&0X7FFF;	//得到数据长度
		for(i=0;i<rxlen;i++)USART1_TX_BUF[i]=USART_RX_BUF[i];	   
		USART_RX_STA=0;		   	//启动下一次接收
		USART1_TX_BUF[i]=0;			//自动添加结束符
		if(!GPS_Analysis(&gpsx,(u8*)USART1_TX_BUF))
		{
			Gps_Msg_Show();				
		}			
	}

  //OSTaskDel(OS_PRIO_SELF);	
}








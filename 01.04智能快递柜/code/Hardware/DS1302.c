/************************************************************************************
//（店名“众明工作室”）淘宝链接: https://shop149406681.taobao.com/?spm=a1z10.1-c.0.0.64cb457fKzgKea 
//淘宝店内搜索关键词：K_A37_005
//  版 本 号   : v1.0
//  作    者   : 众明工作室
//  交流群 :			 1041406448（其它更多资料请关注公告）
//    CSDN  （基本参数，资源更新列表，所有接线说明）   :单片机代码搬运工(搜索关键词：K_A37_005)
//    BILIBILI（演示视频） :众明工作室(搜索关键词：K_A37_005)
//  生成日期   : 2021-6-21
//  最近修改   : 2021-11-21
//  功能描述   : ADS1115块测试程序
//  测试条件   : STM32F103C8T6   晶振8M  系统时钟72M
接线
ADS1115--------------------------------STM32F103C8T6
VCC-- ---------------------------------5V
GND- ----------------------------------GND
SCL- ----------------------------------PB_4 //SCL
SDA- ----------------------------------PB_3 //SDA	

OLED0.96
VCC-- ---------------------------------3.3V
GND- ----------------------------------GND
SCL- ----------------------------------PB_8 //SCL
SDA- ----------------------------------PB_9 //SDA	
*************************************************************************************/

#include "DS1302.h"
#include "delay.h"
#include "oled.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"

//---DS1302写入和读取时分秒的地址命令---//
//---秒分时日月周年 最低位读写位;-------//
uint8_t  READ_RTC_ADDR[7] = {0x81, 0x83, 0x85, 0x87, 0x89, 0x8b, 0x8d}; 
uint8_t  WRITE_RTC_ADDR[7] = {0x80, 0x82, 0x84, 0x86, 0x88, 0x8a, 0x8c};
extern uint8_t D_Buf[11];  //存储年月日
extern uint8_t Time_Buf[10]; //存储时分秒
extern uint8_t year;           //年高两位
extern uint8_t week[2];               //星期
extern uint8_t KEY0_Num;           //切换调整项
u8 TIMELED1_S_NUM;
u8 TIMELED1_M_NUM;
u8 TIMELED1_H_NUM;
extern char TIMELED1_S[100];
extern char TIMELED1_M[100];
extern char TIMELED1_H[100];

//---DS1302时钟初始化2013年1月1日星期二12点00分00秒。---//
//---存储顺序是秒分时日月周年,存储格式是用BCD码---//
uint8_t TIME[7] = {0x20, 0x09, 0x21, 0x17, 0x10, 0x04, 0x24};

/*******************************************************************************
配置成推挽输出模式
*******************************************************************************/
void DS1302_OUT_GPIO_INIT(void)
{

	GPIO_InitTypeDef  GPIO_InitStructure;  

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_3 | GPIO_Pin_4| GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//
	GPIO_Init(GPIOB, &GPIO_InitStructure);

}
/*******************************************************************************
数据引脚配置成上拉输入模式
*******************************************************************************/
void DS1302_IN_GPIO_INIT(void)//DAT
{
    GPIO_InitTypeDef  GPIO_InitStructure;  

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
void Delay1us(void)
{
    delay_us(1);
}
void Ds1302Write(uint8_t addr, uint8_t dat)
{
	uint8_t n;
    DS1302_OUT_GPIO_INIT();
	RST_L;
	Delay1us();
   
	SCLK_L;//先将SCLK置低电平。
	Delay1us();
	RST_H; //然后将RST(CE)置高电平。
	Delay1us();

	for (n=0; n<8; n++)//开始传送八位地址命令
	{
        if(addr & 0x01)
        {
            DSIO_H;//数据从低位开始传送
        }
        else
        {
            DSIO_L;
        }
		addr >>= 1;
		SCLK_H;//数据在上升沿时，DS1302读取数据
		Delay1us();
		SCLK_L;
		Delay1us();
	}
	for (n=0; n<8; n++)//写入8位数据
	{
		if(dat & 0x01)
        {
            DSIO_H;//数据从低位开始传送
        }
        else
        {
            DSIO_L;
        }
		dat >>= 1;
		SCLK_H;//数据在上升沿时，DS1302读取数据
		Delay1us();
		SCLK_L;
		Delay1us();	
	}	
		 
	RST_L;//传送数据结束
	Delay1us();
}

/*******************************************************************************
* 函 数 名         : Ds1302Read
* 函数功能		   : 读取一个地址的数据
* 输    入         : addr
* 输    出         : dat
*******************************************************************************/

uint8_t Ds1302Read(uint8_t addr)
{
	uint8_t n,dat,dat1;
	RST_L;
	Delay1us();

	SCLK_L;//先将SCLK置低电平。
	Delay1us();
	RST_H;//然后将RST(CE)置高电平。
	Delay1us();
    
	for(n=0; n<8; n++)//开始传送八位地址命令
	{
		if(addr & 0x01)
        {
            DSIO_H;//数据从低位开始传送
        }
        else
        {
            DSIO_L;
        }
		addr >>= 1;
		SCLK_H;//数据在上升沿时，DS1302读取数据
		Delay1us();
		SCLK_L;//DS1302下降沿时，放置数据
		Delay1us();
	}
	Delay1us();
    DS1302_IN_GPIO_INIT();
	for(n=0; n<8; n++)//读取8位数据
	{
		dat1 = GET_DSIO;//从最低位开始接收
		dat = (dat>>1) | (dat1<<7);
		SCLK_H;
		Delay1us();
		SCLK_L;//DS1302下降沿时，放置数据
		Delay1us();
	}
    DS1302_OUT_GPIO_INIT();
	RST_L;
	Delay1us();	//以下为DS1302复位的稳定时间,必须的。
	SCLK_H;
	Delay1us();
	DSIO_L;
	Delay1us();
	DSIO_H;
	Delay1us();
	return dat;	
}

/*******************************************************************************
* 函 数 名         : Ds1302Init
* 函数功能		   : 初始化DS1302.
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/

void Ds1302Init(void)
{
	uint8_t n;
	Ds1302Write(0x8E,0X00);		 //禁止写保护，就是关闭写保护功能
	for (n=0; n<7; n++)//写入7个字节的时钟信号：分秒时日月周年
	{
		Ds1302Write(WRITE_RTC_ADDR[n],TIME[n]);	
	}
	Ds1302Write(0x8E,0x80);		 //打开写保护功能
}

/*******************************************************************************
* 函 数 名         : Ds1302ReadTime
* 函数功能		   : 读取时钟信息
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/

void Ds1302ReadTime(void)
{
	uint8_t n;
	for (n=0; n<7; n++)//读取7个字节的时钟信号：分秒时日月周年
	{
		TIME[n] = Ds1302Read(READ_RTC_ADDR[n]);
	}
		
}
void DS1302_Dis()
{
 
		Ds1302ReadTime();
		Time_Buf[7]=TIME[0]%16+0x30;
		Time_Buf[6]=TIME[0]/16+0x30;
		Time_Buf[5]=':';
		/**分显示**/
		Time_Buf[4]=TIME[1]%16+0x30;
		Time_Buf[3]=TIME[1]/16+0x30;

		Time_Buf[2]=':';
		/**时显示**/
		Time_Buf[1]=TIME[2]%16+0x30;
		Time_Buf[0]=TIME[2]/16+0x30;
		Time_Buf[8]='\0';//结束符号
	
	
		/**日显示**/
		D_Buf[9]=TIME[3]%16+0x30;
		D_Buf[8]=TIME[3]/16+0x30;   
		D_Buf[7]='-';    
		/**月显示**/
		D_Buf[6]=TIME[4]%16+0x30;
		D_Buf[5]=TIME[4]/16+0x30;   
		D_Buf[4]='-';    
		/**年低两位显示**/
		D_Buf[3]=TIME[6]%16+0x30;
		D_Buf[2]=TIME[6]/16+0x30;   
		/**年低高位显示**/
		D_Buf[1]=year%10+0x30;
		D_Buf[0]=year/10+0x30;   

		D_Buf[10]='\0';//结束符号
		
		/**周显示**/
//		week[0]=TIME[5]+0x30;
		week[0]=TIME[5];
		week[1] = '\0';
		OLED_ShowString(24,0,(char*)D_Buf,8);
		OLED_ShowChinese(40,16,"星期");
		switch(week[0])
		{
			case 1:OLED_ShowChinese(72,16,"一");break;
			case 2:OLED_ShowChinese(72,16,"二");break;
			case 3:OLED_ShowChinese(72,16,"三");break;
			case 4:OLED_ShowChinese(72,16,"四");break;
			case 5:OLED_ShowChinese(72,16,"五");break;
			case 6:OLED_ShowChinese(72,16,"六");break;
			case 7:OLED_ShowChinese(72,16,"日");break;
			default:OLED_ShowChinese(72,16,"");
		}
//		OLED_ShowChinese();
//		OLED_P16x16Ch(72,2,week[0]);
		OLED_ShowString(30,32,(char*)Time_Buf,8);
		OLED_Update();
		
		
}
void DS1302_Dis1()
{
 
		Ds1302ReadTime();
		Time_Buf[7]=TIME[0]%16+0x30;
		Time_Buf[6]=TIME[0]/16+0x30;
		Time_Buf[5]=':';
		/**分显示**/
		Time_Buf[4]=TIME[1]%16+0x30;
		Time_Buf[3]=TIME[1]/16+0x30;

		Time_Buf[2]=':';
		/**时显示**/
		Time_Buf[1]=TIME[2]%16+0x30;
		Time_Buf[0]=TIME[2]/16+0x30;
		Time_Buf[8]='\0';//结束符号
	
	
		/**日显示**/
		D_Buf[9]=TIME[3]%16+0x30;
		D_Buf[8]=TIME[3]/16+0x30;   
		D_Buf[7]='-';    
		/**月显示**/
		D_Buf[6]=TIME[4]%16+0x30;
		D_Buf[5]=TIME[4]/16+0x30;   
		D_Buf[4]='-';    
		/**年低两位显示**/
		D_Buf[3]=TIME[6]%16+0x30;
		D_Buf[2]=TIME[6]/16+0x30;   
		/**年低高位显示**/
		D_Buf[1]=year%10+0x30;
		D_Buf[0]=year/10+0x30;   

		D_Buf[10]='\0';//结束符号
		
		/**周显示**/
//		week[0]=TIME[5]+0x30;
		week[0]=TIME[5];
		week[1] = '\0';
//		OLED_ShowString(24,0,(char*)D_Buf,8);
//		OLED_ShowChinese(40,16,"星期");
//		switch(week[0])
//		{
//			case 1:OLED_ShowChinese(72,16,"一");break;
//			case 2:OLED_ShowChinese(72,16,"二");break;
//			case 3:OLED_ShowChinese(72,16,"三");break;
//			case 4:OLED_ShowChinese(72,16,"四");break;
//			case 5:OLED_ShowChinese(72,16,"五");break;
//			case 6:OLED_ShowChinese(72,16,"六");break;
//			case 7:OLED_ShowChinese(72,16,"日");break;
//			default:OLED_ShowChinese(72,16,"");
//		}
////		OLED_ShowChinese();
////		OLED_P16x16Ch(72,2,week[0]);
		OLED_ShowString(0,5,(char*)Time_Buf,6);
		OLED_Update();
		
		
}
void DS1302_Dis2()
{
 
		Ds1302ReadTime();
		TIMELED1_S[1]=TIME[0]%16+0x30;
		TIMELED1_S[0]=TIME[0]/16+0x30;
//		TIMELED1_S[2]='\0';
		/**分显示**/
		TIMELED1_M[1]=TIME[1]%16+0x30;
		TIMELED1_M[0]=TIME[1]/16+0x30;
//	TIMELED1_M[2]='\0';
		/**时显示**/
		TIMELED1_H[1]=TIME[2]%16+0x30;
		TIMELED1_H[0]=TIME[2]/16+0x30;
//	TIMELED1_H[2]='\0';
		TIMELED1_S_NUM=atoi(TIMELED1_S);
		TIMELED1_M_NUM=atoi(TIMELED1_M);
		TIMELED1_H_NUM=atoi(TIMELED1_H);
}


































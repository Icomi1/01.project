#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "i2c.h"
#include "i2c.h"
#include "bsp.h"
#include "oled.h"
#include "DHT11.h"
#include "stdio.h"
#include "string.h"


//velocity
#define slowly_time 180
#define quick_time 100


//USART
_Bool mode=0;//模式选择
uint8_t rxdat;
char rxdata[20];
uint8_t rx_pointer;
char interset[20];

uint8_t rxdat2;
char tx_tp_diff[2];

//Status
uint8_t face_pointer;//表情
uint8_t run_status;//运动
_Bool leisure_flag;//休眠

//sleep
_Bool connect1_flag;
_Bool connect1_lost_flag;


//dht11
uint16_t temperature;
uint16_t humidity;

        
//hcsr04
uint8_t  TIM4CH1_CAPTURE_STA;                
uint16_t TIM4CH1_CAPTURE_VAL;
uint8_t len;
uint32_t time;

//LED
_Bool led_num5;


//interseting
uint8_t physical=100;

//time
uint16_t sec1_time;
uint8_t sys_time[3];


uint32_t rx_time=0;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
   if(huart->Instance==USART1)
   {
      rx_time = uwTick;
      rxdata[rx_pointer++]=rxdat;
      HAL_UART_Receive_IT(&huart1,&rxdat,1);
   }
   if(huart->Instance==USART2)
   {
      rx_time = uwTick;
      HAL_UART_Receive_IT(&huart2,&rxdat2,1);
   }
}


_Bool rxchar;
void Rx_Display()
{
   if(uwTick-rx_time<80) return;
   rx_time = uwTick;
   
   if(rx_pointer>0)
   {
      if(rx_pointer==1)
      {
         if(rxdat=='a'&& mode!=1 && physical > 10)//开启语言模式
         {
            OLED_NewFrame();
            face_pointer=3;
            leisure_flag=0;
            run_status=0;
            mode=1;
         }
         else if(rxdat=='b'&& mode!=0 && physical > 10)//开启蓝牙模式(默认)
         {
            OLED_NewFrame();
            face_pointer=0;
            leisure_flag=0;
            run_status=0;
            mode=0;
         }
         if(!mode)//==================================================================蓝牙
         {
            if(physical > 10)//体力限制
            {
               if(rxdat=='0' && run_status!=0)//'0'!=0
               {
                  OLED_NewFrame();
                  face_pointer=0;
                  run_status=0;//立正
                  physical-=2;
               }
               else if(rxdat=='1' && run_status!=1)
               {
                  OLED_NewFrame();
                  face_pointer=0;
                  run_status=1;
                  physical-=5;
               }
               else if(rxdat=='2'&& run_status!=2)
               {
                  OLED_NewFrame();
                  face_pointer=0;
                  run_status=2;
                  physical-=5;
               }
               else if(rxdat=='3'&& run_status!=3)
               {
                  OLED_NewFrame();
                  face_pointer=0;
                  run_status=3;
                  physical-=5;
               }
               else if(rxdat=='4'&& run_status!=4)
               {
                  OLED_NewFrame();
                  face_pointer=0;
                  run_status=4;
                  physical-=5;
               }
               else if(rxdat=='5'&& run_status!=5)//摇摆模式
               {
                  OLED_NewFrame();
                  face_pointer=4;
                  run_status=5;
                  physical-=8;
               }
               else if(rxdat=='6'&& run_status!=6)//招手模式
               {
                  OLED_NewFrame();
                  face_pointer=1;
                  run_status=6;
                  physical-=3;
               }
               else if(rxdat=='7'&& run_status!=7)//坐下模式
               {
                  OLED_NewFrame();
                  face_pointer=1;
                  run_status=7;
                  physical-=3;
               }
               else if(rxdat=='8'&& leisure_flag!=1)//休闲模式
               {
                  OLED_NewFrame();
                  run_status=8;
               }
               else if(rxdat=='9'&& run_status!=9)//跟随模式
               {
                  OLED_NewFrame();
                  face_pointer=2;
                  run_status=9;
                  physical-=5;
               }
            }
         }
      }
      else if(rxdata[0]=='@')
      {
         rxchar=1;
         led_num5=1;
         memset(&interset,0,sizeof(interset));
         for(uint8_t i = 1;i < 20;i++)
         {
            interset[i-1] = rxdata[i];
         }
      }
      rx_pointer=0;
      memset(&rxdata,0,sizeof(rxdata));
   }
   if(mode)//============================================================================语言模式(mode=1)
   {
      if(physical > 10)
      {
         if(rxdat2=='0' && run_status!=0)//'0'!=0
         {
            OLED_NewFrame();
            face_pointer=3;
            run_status=0;//立正
            physical-=2;
         }
         else if(rxdat2=='1' && run_status!=1)
         {
            OLED_NewFrame();
            face_pointer=3;
            run_status=1;
            physical-=5;
         }
         else if(rxdat2=='2'&& run_status!=2)
         {
            OLED_NewFrame();
            face_pointer=3;
            run_status=2;
            physical-=5;
         }
         else if(rxdat2=='3'&& run_status!=3)
         {
            OLED_NewFrame();
            face_pointer=3;
            run_status=3;
            physical-=5;
         }
         else if(rxdat2=='4'&& run_status!=4)
         {
            OLED_NewFrame();
            face_pointer=3;
            run_status=4;
            physical-=5;
         }
         else if(rxdat2=='5'&& run_status!=5)
         {
            OLED_NewFrame();
            face_pointer=3;
            run_status=5;
            physical-=5;
         }
      }
   }
    //温度
      if(rxdat2=='t')
      {
         DHT11_Read_Data(&temperature,&humidity);
         sprintf(tx_tp_diff,"%d",temperature>>8);
         HAL_UART_Transmit_IT(&huart2,(uint8_t*)tx_tp_diff,2);
      }
      //(接收内容清除)
      rxdat2=0;
}


//0~8状态执行
uint32_t exercise_time=0;
void Exercise()
{
   if(uwTick-exercise_time<100) return;
   exercise_time = uwTick;
   
   if(connect1_flag || mode )//蓝牙连接状态或者处于语音模式
   {
      if(run_status!=8 && run_status!=10)
      {
         leisure_flag=0;
      }
      if(run_status==0)
      {
         Stand();
      }
      else if(run_status==1)
      {
         Advance();
      }
      else if(run_status==2)
      {
         Retreat();
      }
      else if(run_status==3)
      {
         Left();
      }
      else if(run_status==4)
      {
         Right();
      }
      else if(run_status==5)//摇摆
      {
         Shake();
      }
      else if(run_status==6)//招手(需要复位)
      {
         Hello();
         run_status=10;//进入空白
         face_pointer=0;
      }
      else if(run_status==7)//坐下(需要复位)
      {
         Sit_down();
         run_status=10;//进入空白
         face_pointer=1;
      }
      else if(run_status==8)//休闲
      {
         Get_down1();
         leisure_flag=1;//休闲画面
         run_status=10;//进入空白
      }
      else if(run_status==9)//跟随
      {
         Talk();
      }
      else
      {
         //空白执行
      }
   }
}





uint8_t yid_num;
uint8_t yid_num2;
uint8_t oled_sx_dhttime;
uint32_t oled_time=0;
void OLED_Display()
{
   if(uwTick-oled_time<200) return;
   oled_time = uwTick;
   
   char dispBuff[25];
   char oled_diff[25];
  char oled_rtc[25];
   if(leisure_flag==0)
   {
      Face_proc();
      OLED_ShowFrame();
   }
   else            //休闲画面
   {
      oled_sx_dhttime++;
      if(oled_sx_dhttime%2)//每400ms
      {
         Read_Distance();
         if(len>50)
         {
            len=50;
         }
         else if(len<=2)
         {
            len=2;
         }
      }
      if(oled_sx_dhttime%5)//每1000ms
      {
         oled_sx_dhttime=0;
         DHT11_Read_Data(&temperature,&humidity);
      }
      OLED_NewFrame();
      
      sprintf(dispBuff,"Temp:%d.%d' Humi:%d.%d%%",temperature>>8,temperature&0xff,humidity>>8,humidity&0xff);
      sprintf(oled_diff,"Phy:%d  Dis:%dcm",physical,len);//体力值
      sprintf(oled_rtc,"Runing:%d:%d:%d",sys_time[0],sys_time[1],sys_time[2]);//RTC
      
      OLED_PrintASCIIString(1,1,dispBuff,&afont12x6,OLED_COLOR_NORMAL);
      OLED_PrintASCIIString(1,15,oled_diff,&afont12x6,OLED_COLOR_NORMAL);
      OLED_PrintASCIIString(1,30,oled_rtc,&afont12x6,OLED_COLOR_NORMAL);
      
      if(rxchar)
      {
         OLED_PrintASCIIString(128-2*yid_num,45,interset,&afont16x8,OLED_COLOR_NORMAL);
         yid_num=(yid_num+4)%132;
         if(yid_num==0)//表示展示一次
         {
            yid_num2++;
            if(yid_num2==10)
            {
               rxchar=0;
               yid_num2=0;
            }
         }
      }
      else
         OLED_PrintASCIIString(1,45,"",&afont16x8,OLED_COLOR_NORMAL);
      OLED_ShowFrame();
      
   }
}




void Face_proc()
{
   if(face_pointer==0)
      OLED_DrawImage(0,0,&Q1Img,OLED_COLOR_NORMAL);//默认表情
   else  if(face_pointer==1)
      OLED_DrawImage(0,0,&WX1Img,OLED_COLOR_NORMAL);//微笑
   else  if(face_pointer==2)
      OLED_DrawImage(0,0,&KQ1Img,OLED_COLOR_NORMAL);//哭泣
   else  if(face_pointer==3)
      OLED_DrawImage(0,0,&YY1Img,OLED_COLOR_NORMAL);//语音
   else  if(face_pointer==4)
      OLED_DrawImage(0,0,&JTImg,OLED_COLOR_NORMAL);//眯眼
   else  if(face_pointer==5)
      OLED_DrawImage(0,0,&XM1Img,OLED_COLOR_NORMAL);//休眠1
}




uint8_t physical_lost;
uint8_t phy_time_num;
uint32_t phy_time=0;
void Phy_Display()
{
   if(uwTick-phy_time<500) return;
   phy_time = uwTick;
   
   if(physical<=10 && physical_lost>10)//自动进入休眠
   {
      HAL_UART_Transmit_IT(&huart2,(uint8_t*)"sleep",5);
      OLED_NewFrame();
      leisure_flag=1;
      Get_down1();
      run_status=10;
   }
   physical_lost = physical;
   
   if(leisure_flag)//休眠画面下
   {
      phy_time_num++;
      if(phy_time_num>=10)
      {
         phy_time_num=0;
         if(physical<100)
            physical++;
      }
   }
}



uint32_t sleep_time=0;
void Sleep_Display()
{
   if(uwTick-sleep_time<500) return;
   sleep_time = uwTick;
   
   if(HAL_GPIO_ReadPin(GPIOA,USART_EN_Pin)==0)
      connect1_flag=0;
   else
      connect1_flag=1;
   
      if(connect1_flag==0 && connect1_lost_flag==1 && mode == 0)//失联
      {
         //在休闲模式下，进入离线，可以保持休眠
         led_num5=1;
         OLED_NewFrame();
         face_pointer=5;
         if(!leisure_flag)
         Get_down1();
         run_status=10;//进入空白
      }
      else if(connect1_flag==1 && connect1_lost_flag==0 )//再连
      {
         //退出离线，退出休眠
         led_num5=1;
         leisure_flag=0;
         OLED_NewFrame();
         if(!mode)
            face_pointer=0;
         else
            face_pointer=3;
         run_status=0;//立正
      }
   connect1_lost_flag = connect1_flag;
}



_Bool qwq_flag;
void Talk()
{
      Advance();
      qwq_flag=!qwq_flag;
      Read_Distance();
    if(len<15)
      {
         if(qwq_flag)
         {
            Retreat();
            Retreat();
            Left();
            Left();
            Left();
            Left();
            Left();
            Read_Distance();
         }
         else
         {
            Retreat();
            Retreat();
            Right();
            Right();
            Right();
            Right();
            Right();
            Read_Distance();
         }
      }
}



uint8_t led_num5_pointer=0;
uint32_t led_time=0;
void LED_Display()
{
   if(uwTick-led_time<80) return;
   led_time = uwTick;
   
   if(led_num5)
   {
      led_num5_pointer++;
      HAL_GPIO_TogglePin(GPIOC,LED_Pin);
      if(led_num5_pointer==10)
      {
         led_num5=0;
         led_num5_pointer=0;
         HAL_GPIO_WritePin(GPIOC,LED_Pin,GPIO_PIN_SET);
      }
   }
}




void Read_Distance()
{
   uint32_t i;
   
   HAL_GPIO_WritePin(TRIG_GPIO_Port, TRIG_Pin, GPIO_PIN_SET);
   for(i=0;i<72*40;i++)
      __NOP();
   HAL_GPIO_WritePin(TRIG_GPIO_Port, TRIG_Pin, GPIO_PIN_RESET);
   if(TIM4CH1_CAPTURE_STA & 0x80)   //成功捕获一个脉宽
   {
      time = TIM4CH1_CAPTURE_STA & 0x3F;   //获取溢出次数
      time *= 65536;      //获得溢出的时间值
      time += TIM4CH1_CAPTURE_VAL;   //加上最后一次取得的值
      len = time * 342.62*100/2000000;
      TIM4CH1_CAPTURE_STA = 0;
   }
}



uint32_t sy_time=0;
void Sy_proc()
{
   if(uwTick-sy_time<200) return;
   sy_time = uwTick;
   
   if(sys_time[2]>=60)
   {
      sys_time[2]=0;
      sys_time[1]++;
      if(sys_time[1]>=60)
      {
         sys_time[1]=0;
         sys_time[0]++;
         if(sys_time[0]==24)
         {
            sys_time[2]=sys_time[1]=sys_time[0]=0;
            uwTick=0;
         }
      }
   }
}





//设置SG90为参数范围内(0~180°)任意度数
/*
* htim:要设置的定时器句柄
* Channel：PWM输出通道
* Arr：PWM一个周期计数值
* angle:角度
*
*/
void SG90_dispaly(TIM_HandleTypeDef * htim,uint32_t Channel, uint32_t Arr, short angle)
{
    uint16_t duty = Arr * ((0.0556f * angle + 7.5f)/100.0f);
    __HAL_TIM_SET_COMPARE(htim,Channel,duty);
}


//======================================================超声波中断====================================================================

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)//更新中断（溢出）发生时执行
{
   if(htim->Instance==TIM1)
   {
      if((TIM4CH1_CAPTURE_STA&0X80) ==0)      //还未捕获成功
      {
         if(TIM4CH1_CAPTURE_STA&0X40)      //捕获到一个下降沿
         {
            if((TIM4CH1_CAPTURE_STA&0X3F)==0X3F)   //高电平时间太长了
            {
               TIM4CH1_CAPTURE_STA |= 0X80;   //标记成功捕获一次
               TIM4CH1_CAPTURE_VAL = 0XFFFF;   //
            }
            
            else
               TIM4CH1_CAPTURE_STA++;         //否则标记溢出数加一
         }
      }
   }
}
 
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)//捕获中断发生时执行
{
   if(htim->Instance==TIM1)
   {
      if((TIM4CH1_CAPTURE_STA&0x80)==0)   //还未捕获成功
      {
         if(TIM4CH1_CAPTURE_STA&0x40)   //成功捕获到一个下降沿
         {
            TIM4CH1_CAPTURE_STA |= 0X80;   //标记成功，捕获到一次高电平完成
            TIM4CH1_CAPTURE_VAL  = HAL_TIM_ReadCapturedValue(&htim1, TIM_CHANNEL_1);   //获取当前捕获值
            TIM_RESET_CAPTUREPOLARITY(&htim1, TIM_CHANNEL_1);         //清除原来设置
            TIM_SET_CAPTUREPOLARITY(&htim1, TIM_CHANNEL_1, TIM_ICPOLARITY_RISING);   //捕获到下降沿后，将捕获复位到捕获上升沿
         }
         
         else                     //捕获到一个上升沿
         {
            TIM4CH1_CAPTURE_STA = 0;
            TIM4CH1_CAPTURE_VAL = 0;
            TIM4CH1_CAPTURE_STA |= 0x40;   //第六位标记为捕获到上升沿
            __HAL_TIM_DISABLE(&htim1);      //关闭定时器
            __HAL_TIM_SET_COUNTER(&htim1, 0);   //定时器初始值设置为0
            TIM_RESET_CAPTUREPOLARITY(&htim1, TIM_CHANNEL_1);
            TIM_SET_CAPTUREPOLARITY(&htim1, TIM_CHANNEL_1, TIM_ICPOLARITY_FALLING);   //捕获到上升沿之后，将捕获设置为下降沿
            __HAL_TIM_ENABLE(&htim1);
         }
      }
         HAL_TIM_IC_Start_IT(&htim1,TIM_CHANNEL_1);
   }
}




//==============================================动作代码区===================================================================

//前进
void Advance()
{
   SG90_dispaly(&htim2,TIM_CHANNEL_1,20000,45);
   SG90_dispaly(&htim2,TIM_CHANNEL_3,20000,-45);
   HAL_Delay(slowly_time);
   SG90_dispaly(&htim2,TIM_CHANNEL_2,20000,-45);
   SG90_dispaly(&htim2,TIM_CHANNEL_4,20000,45);
   HAL_Delay(slowly_time);
   SG90_dispaly(&htim2,TIM_CHANNEL_1,20000,0);
   SG90_dispaly(&htim2,TIM_CHANNEL_3,20000,0);
   HAL_Delay(slowly_time);
   SG90_dispaly(&htim2,TIM_CHANNEL_2,20000,0);
   SG90_dispaly(&htim2,TIM_CHANNEL_4,20000,0);
   HAL_Delay(slowly_time);
   SG90_dispaly(&htim2,TIM_CHANNEL_2,20000,45);
   SG90_dispaly(&htim2,TIM_CHANNEL_4,20000,-45);
   HAL_Delay(slowly_time);
   SG90_dispaly(&htim2,TIM_CHANNEL_1,20000,-45);
   SG90_dispaly(&htim2,TIM_CHANNEL_3,20000,45);
   HAL_Delay(slowly_time);
   SG90_dispaly(&htim2,TIM_CHANNEL_2,20000,0);
   SG90_dispaly(&htim2,TIM_CHANNEL_4,20000,0);
   HAL_Delay(slowly_time);
   SG90_dispaly(&htim2,TIM_CHANNEL_1,20000,0);
   SG90_dispaly(&htim2,TIM_CHANNEL_3,20000,0);
   HAL_Delay(quick_time);
}



//后退
void Retreat()
{
   SG90_dispaly(&htim2,TIM_CHANNEL_1,20000,-45);
   SG90_dispaly(&htim2,TIM_CHANNEL_3,20000,45);
   HAL_Delay(slowly_time);
   SG90_dispaly(&htim2,TIM_CHANNEL_2,20000,45);
   SG90_dispaly(&htim2,TIM_CHANNEL_4,20000,-45);
   HAL_Delay(slowly_time);
   SG90_dispaly(&htim2,TIM_CHANNEL_1,20000,0);
   SG90_dispaly(&htim2,TIM_CHANNEL_3,20000,0);
   HAL_Delay(slowly_time);
   SG90_dispaly(&htim2,TIM_CHANNEL_2,20000,0);
   SG90_dispaly(&htim2,TIM_CHANNEL_4,20000,0);
   HAL_Delay(slowly_time);
   SG90_dispaly(&htim2,TIM_CHANNEL_2,20000,-45);
   SG90_dispaly(&htim2,TIM_CHANNEL_4,20000,45);
   HAL_Delay(slowly_time);
   SG90_dispaly(&htim2,TIM_CHANNEL_1,20000,45);
   SG90_dispaly(&htim2,TIM_CHANNEL_3,20000,-45);
   HAL_Delay(slowly_time);
   SG90_dispaly(&htim2,TIM_CHANNEL_2,20000,0);
   SG90_dispaly(&htim2,TIM_CHANNEL_4,20000,0);
   HAL_Delay(slowly_time);
   SG90_dispaly(&htim2,TIM_CHANNEL_1,20000,0);
   SG90_dispaly(&htim2,TIM_CHANNEL_3,20000,0);
   HAL_Delay(quick_time);
}



//左转
void Left()
{
   SG90_dispaly(&htim2,TIM_CHANNEL_2,20000,60);
   SG90_dispaly(&htim2,TIM_CHANNEL_4,20000,60);
   HAL_Delay(slowly_time);
   SG90_dispaly(&htim2,TIM_CHANNEL_1,20000,-60);
   SG90_dispaly(&htim2,TIM_CHANNEL_3,20000,-60);
   HAL_Delay(slowly_time);
   SG90_dispaly(&htim2,TIM_CHANNEL_2,20000,0);
   SG90_dispaly(&htim2,TIM_CHANNEL_4,20000,0);
   HAL_Delay(slowly_time);
   SG90_dispaly(&htim2,TIM_CHANNEL_1,20000,0);
   SG90_dispaly(&htim2,TIM_CHANNEL_3,20000,0);
   HAL_Delay(quick_time);
}


//右转
void Right()
{
   SG90_dispaly(&htim2,TIM_CHANNEL_1,20000,-60);
   SG90_dispaly(&htim2,TIM_CHANNEL_3,20000,-60);
   HAL_Delay(slowly_time);
   SG90_dispaly(&htim2,TIM_CHANNEL_2,20000,60);
   SG90_dispaly(&htim2,TIM_CHANNEL_4,20000,60);
   HAL_Delay(slowly_time);
   SG90_dispaly(&htim2,TIM_CHANNEL_1,20000,0);
   SG90_dispaly(&htim2,TIM_CHANNEL_3,20000,0);
   HAL_Delay(slowly_time);
   SG90_dispaly(&htim2,TIM_CHANNEL_2,20000,0);
   SG90_dispaly(&htim2,TIM_CHANNEL_4,20000,0);
   HAL_Delay(quick_time);
}

//站立
void Stand()
{
   SG90_dispaly(&htim2,TIM_CHANNEL_1,20000,0);
   SG90_dispaly(&htim2,TIM_CHANNEL_2,20000,0);
   SG90_dispaly(&htim2,TIM_CHANNEL_3,20000,0);
   SG90_dispaly(&htim2,TIM_CHANNEL_4,20000,0);
}

//趴下
void Get_down1()//前前后后
{
   for(uint8_t i = 0;i<=80;i+=5)
   {
      SG90_dispaly(&htim2,TIM_CHANNEL_1,20000,i);
      SG90_dispaly(&htim2,TIM_CHANNEL_2,20000,-i);
      SG90_dispaly(&htim2,TIM_CHANNEL_3,20000,i);
      SG90_dispaly(&htim2,TIM_CHANNEL_4,20000,-i);
      HAL_Delay(30);
   }
}



//摇摆
void Shake()
{
   SG90_dispaly(&htim2,TIM_CHANNEL_1,20000,60);
   SG90_dispaly(&htim2,TIM_CHANNEL_2,20000,60);
   SG90_dispaly(&htim2,TIM_CHANNEL_3,20000,-60);
   SG90_dispaly(&htim2,TIM_CHANNEL_4,20000,-60);
   HAL_Delay(slowly_time);
   SG90_dispaly(&htim2,TIM_CHANNEL_1,20000,0);
   SG90_dispaly(&htim2,TIM_CHANNEL_2,20000,0);
   SG90_dispaly(&htim2,TIM_CHANNEL_3,20000,0);
   SG90_dispaly(&htim2,TIM_CHANNEL_4,20000,0);
   HAL_Delay(slowly_time);
   SG90_dispaly(&htim2,TIM_CHANNEL_1,20000,-60);
   SG90_dispaly(&htim2,TIM_CHANNEL_2,20000,-60);
   SG90_dispaly(&htim2,TIM_CHANNEL_3,20000,60);
   SG90_dispaly(&htim2,TIM_CHANNEL_4,20000,60);
   HAL_Delay(slowly_time);
   SG90_dispaly(&htim2,TIM_CHANNEL_1,20000,0);
   SG90_dispaly(&htim2,TIM_CHANNEL_2,20000,0);
   SG90_dispaly(&htim2,TIM_CHANNEL_3,20000,0);
   SG90_dispaly(&htim2,TIM_CHANNEL_4,20000,0);
   HAL_Delay(quick_time);
}


//招手
void Hello()
{
   SG90_dispaly(&htim2,TIM_CHANNEL_1,20000,0);
   SG90_dispaly(&htim2,TIM_CHANNEL_2,20000,0);
   SG90_dispaly(&htim2,TIM_CHANNEL_3,20000,0);
   SG90_dispaly(&htim2,TIM_CHANNEL_4,20000,0);
   HAL_Delay(slowly_time);
   SG90_dispaly(&htim2,TIM_CHANNEL_1,20000,90);
   HAL_Delay(slowly_time);
   SG90_dispaly(&htim2,TIM_CHANNEL_1,20000,60);
   HAL_Delay(slowly_time);
   SG90_dispaly(&htim2,TIM_CHANNEL_1,20000,90);
   HAL_Delay(slowly_time);
   SG90_dispaly(&htim2,TIM_CHANNEL_1,20000,60);
   HAL_Delay(slowly_time);
   SG90_dispaly(&htim2,TIM_CHANNEL_1,20000,90);
   HAL_Delay(slowly_time);
   SG90_dispaly(&htim2,TIM_CHANNEL_1,20000,0);
}


//坐下
void Sit_down()
{
   SG90_dispaly(&htim2,TIM_CHANNEL_1,20000,0);
   SG90_dispaly(&htim2,TIM_CHANNEL_4,20000,0);
   HAL_Delay(500);
   SG90_dispaly(&htim2,TIM_CHANNEL_2,20000,60);
   SG90_dispaly(&htim2,TIM_CHANNEL_3,20000,-60);
   HAL_Delay(500);
}





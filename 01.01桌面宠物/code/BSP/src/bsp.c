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
_Bool mode=0;//ģʽѡ��
uint8_t rxdat;
char rxdata[20];
uint8_t rx_pointer;
char interset[20];

uint8_t rxdat2;
char tx_tp_diff[2];

//Status
uint8_t face_pointer;//����
uint8_t run_status;//�˶�
_Bool leisure_flag;//����

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
         if(rxdat=='a'&& mode!=1 && physical > 10)//��������ģʽ
         {
            OLED_NewFrame();
            face_pointer=3;
            leisure_flag=0;
            run_status=0;
            mode=1;
         }
         else if(rxdat=='b'&& mode!=0 && physical > 10)//��������ģʽ(Ĭ��)
         {
            OLED_NewFrame();
            face_pointer=0;
            leisure_flag=0;
            run_status=0;
            mode=0;
         }
         if(!mode)//==================================================================����
         {
            if(physical > 10)//��������
            {
               if(rxdat=='0' && run_status!=0)//'0'!=0
               {
                  OLED_NewFrame();
                  face_pointer=0;
                  run_status=0;//����
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
               else if(rxdat=='5'&& run_status!=5)//ҡ��ģʽ
               {
                  OLED_NewFrame();
                  face_pointer=4;
                  run_status=5;
                  physical-=8;
               }
               else if(rxdat=='6'&& run_status!=6)//����ģʽ
               {
                  OLED_NewFrame();
                  face_pointer=1;
                  run_status=6;
                  physical-=3;
               }
               else if(rxdat=='7'&& run_status!=7)//����ģʽ
               {
                  OLED_NewFrame();
                  face_pointer=1;
                  run_status=7;
                  physical-=3;
               }
               else if(rxdat=='8'&& leisure_flag!=1)//����ģʽ
               {
                  OLED_NewFrame();
                  run_status=8;
               }
               else if(rxdat=='9'&& run_status!=9)//����ģʽ
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
   if(mode)//============================================================================����ģʽ(mode=1)
   {
      if(physical > 10)
      {
         if(rxdat2=='0' && run_status!=0)//'0'!=0
         {
            OLED_NewFrame();
            face_pointer=3;
            run_status=0;//����
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
    //�¶�
      if(rxdat2=='t')
      {
         DHT11_Read_Data(&temperature,&humidity);
         sprintf(tx_tp_diff,"%d",temperature>>8);
         HAL_UART_Transmit_IT(&huart2,(uint8_t*)tx_tp_diff,2);
      }
      //(�����������)
      rxdat2=0;
}


//0~8״ִ̬��
uint32_t exercise_time=0;
void Exercise()
{
   if(uwTick-exercise_time<100) return;
   exercise_time = uwTick;
   
   if(connect1_flag || mode )//��������״̬���ߴ�������ģʽ
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
      else if(run_status==5)//ҡ��
      {
         Shake();
      }
      else if(run_status==6)//����(��Ҫ��λ)
      {
         Hello();
         run_status=10;//����հ�
         face_pointer=0;
      }
      else if(run_status==7)//����(��Ҫ��λ)
      {
         Sit_down();
         run_status=10;//����հ�
         face_pointer=1;
      }
      else if(run_status==8)//����
      {
         Get_down1();
         leisure_flag=1;//���л���
         run_status=10;//����հ�
      }
      else if(run_status==9)//����
      {
         Talk();
      }
      else
      {
         //�հ�ִ��
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
   else            //���л���
   {
      oled_sx_dhttime++;
      if(oled_sx_dhttime%2)//ÿ400ms
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
      if(oled_sx_dhttime%5)//ÿ1000ms
      {
         oled_sx_dhttime=0;
         DHT11_Read_Data(&temperature,&humidity);
      }
      OLED_NewFrame();
      
      sprintf(dispBuff,"Temp:%d.%d' Humi:%d.%d%%",temperature>>8,temperature&0xff,humidity>>8,humidity&0xff);
      sprintf(oled_diff,"Phy:%d  Dis:%dcm",physical,len);//����ֵ
      sprintf(oled_rtc,"Runing:%d:%d:%d",sys_time[0],sys_time[1],sys_time[2]);//RTC
      
      OLED_PrintASCIIString(1,1,dispBuff,&afont12x6,OLED_COLOR_NORMAL);
      OLED_PrintASCIIString(1,15,oled_diff,&afont12x6,OLED_COLOR_NORMAL);
      OLED_PrintASCIIString(1,30,oled_rtc,&afont12x6,OLED_COLOR_NORMAL);
      
      if(rxchar)
      {
         OLED_PrintASCIIString(128-2*yid_num,45,interset,&afont16x8,OLED_COLOR_NORMAL);
         yid_num=(yid_num+4)%132;
         if(yid_num==0)//��ʾչʾһ��
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
      OLED_DrawImage(0,0,&Q1Img,OLED_COLOR_NORMAL);//Ĭ�ϱ���
   else  if(face_pointer==1)
      OLED_DrawImage(0,0,&WX1Img,OLED_COLOR_NORMAL);//΢Ц
   else  if(face_pointer==2)
      OLED_DrawImage(0,0,&KQ1Img,OLED_COLOR_NORMAL);//����
   else  if(face_pointer==3)
      OLED_DrawImage(0,0,&YY1Img,OLED_COLOR_NORMAL);//����
   else  if(face_pointer==4)
      OLED_DrawImage(0,0,&JTImg,OLED_COLOR_NORMAL);//����
   else  if(face_pointer==5)
      OLED_DrawImage(0,0,&XM1Img,OLED_COLOR_NORMAL);//����1
}




uint8_t physical_lost;
uint8_t phy_time_num;
uint32_t phy_time=0;
void Phy_Display()
{
   if(uwTick-phy_time<500) return;
   phy_time = uwTick;
   
   if(physical<=10 && physical_lost>10)//�Զ���������
   {
      HAL_UART_Transmit_IT(&huart2,(uint8_t*)"sleep",5);
      OLED_NewFrame();
      leisure_flag=1;
      Get_down1();
      run_status=10;
   }
   physical_lost = physical;
   
   if(leisure_flag)//���߻�����
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
   
      if(connect1_flag==0 && connect1_lost_flag==1 && mode == 0)//ʧ��
      {
         //������ģʽ�£��������ߣ����Ա�������
         led_num5=1;
         OLED_NewFrame();
         face_pointer=5;
         if(!leisure_flag)
         Get_down1();
         run_status=10;//����հ�
      }
      else if(connect1_flag==1 && connect1_lost_flag==0 )//����
      {
         //�˳����ߣ��˳�����
         led_num5=1;
         leisure_flag=0;
         OLED_NewFrame();
         if(!mode)
            face_pointer=0;
         else
            face_pointer=3;
         run_status=0;//����
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
   if(TIM4CH1_CAPTURE_STA & 0x80)   //�ɹ�����һ������
   {
      time = TIM4CH1_CAPTURE_STA & 0x3F;   //��ȡ�������
      time *= 65536;      //��������ʱ��ֵ
      time += TIM4CH1_CAPTURE_VAL;   //�������һ��ȡ�õ�ֵ
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





//����SG90Ϊ������Χ��(0~180��)�������
/*
* htim:Ҫ���õĶ�ʱ�����
* Channel��PWM���ͨ��
* Arr��PWMһ�����ڼ���ֵ
* angle:�Ƕ�
*
*/
void SG90_dispaly(TIM_HandleTypeDef * htim,uint32_t Channel, uint32_t Arr, short angle)
{
    uint16_t duty = Arr * ((0.0556f * angle + 7.5f)/100.0f);
    __HAL_TIM_SET_COMPARE(htim,Channel,duty);
}


//======================================================�������ж�====================================================================

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)//�����жϣ����������ʱִ��
{
   if(htim->Instance==TIM1)
   {
      if((TIM4CH1_CAPTURE_STA&0X80) ==0)      //��δ����ɹ�
      {
         if(TIM4CH1_CAPTURE_STA&0X40)      //����һ���½���
         {
            if((TIM4CH1_CAPTURE_STA&0X3F)==0X3F)   //�ߵ�ƽʱ��̫����
            {
               TIM4CH1_CAPTURE_STA |= 0X80;   //��ǳɹ�����һ��
               TIM4CH1_CAPTURE_VAL = 0XFFFF;   //
            }
            
            else
               TIM4CH1_CAPTURE_STA++;         //�������������һ
         }
      }
   }
}
 
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)//�����жϷ���ʱִ��
{
   if(htim->Instance==TIM1)
   {
      if((TIM4CH1_CAPTURE_STA&0x80)==0)   //��δ����ɹ�
      {
         if(TIM4CH1_CAPTURE_STA&0x40)   //�ɹ�����һ���½���
         {
            TIM4CH1_CAPTURE_STA |= 0X80;   //��ǳɹ�������һ�θߵ�ƽ���
            TIM4CH1_CAPTURE_VAL  = HAL_TIM_ReadCapturedValue(&htim1, TIM_CHANNEL_1);   //��ȡ��ǰ����ֵ
            TIM_RESET_CAPTUREPOLARITY(&htim1, TIM_CHANNEL_1);         //���ԭ������
            TIM_SET_CAPTUREPOLARITY(&htim1, TIM_CHANNEL_1, TIM_ICPOLARITY_RISING);   //�����½��غ󣬽�����λ������������
         }
         
         else                     //����һ��������
         {
            TIM4CH1_CAPTURE_STA = 0;
            TIM4CH1_CAPTURE_VAL = 0;
            TIM4CH1_CAPTURE_STA |= 0x40;   //����λ���Ϊ����������
            __HAL_TIM_DISABLE(&htim1);      //�رն�ʱ��
            __HAL_TIM_SET_COUNTER(&htim1, 0);   //��ʱ����ʼֵ����Ϊ0
            TIM_RESET_CAPTUREPOLARITY(&htim1, TIM_CHANNEL_1);
            TIM_SET_CAPTUREPOLARITY(&htim1, TIM_CHANNEL_1, TIM_ICPOLARITY_FALLING);   //����������֮�󣬽���������Ϊ�½���
            __HAL_TIM_ENABLE(&htim1);
         }
      }
         HAL_TIM_IC_Start_IT(&htim1,TIM_CHANNEL_1);
   }
}




//==============================================����������===================================================================

//ǰ��
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



//����
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



//��ת
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


//��ת
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

//վ��
void Stand()
{
   SG90_dispaly(&htim2,TIM_CHANNEL_1,20000,0);
   SG90_dispaly(&htim2,TIM_CHANNEL_2,20000,0);
   SG90_dispaly(&htim2,TIM_CHANNEL_3,20000,0);
   SG90_dispaly(&htim2,TIM_CHANNEL_4,20000,0);
}

//ſ��
void Get_down1()//ǰǰ���
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



//ҡ��
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


//����
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


//����
void Sit_down()
{
   SG90_dispaly(&htim2,TIM_CHANNEL_1,20000,0);
   SG90_dispaly(&htim2,TIM_CHANNEL_4,20000,0);
   HAL_Delay(500);
   SG90_dispaly(&htim2,TIM_CHANNEL_2,20000,60);
   SG90_dispaly(&htim2,TIM_CHANNEL_3,20000,-60);
   HAL_Delay(500);
}





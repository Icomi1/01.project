#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "csb.h"
#include "usart.h"
#include "pwm.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "adc_v.h"
#include "Encoder.h"
#include "pid.h"
#include "led.h"
#include "key.h"
#include "Avoidance.h"
#include "GUI.h"



int main(void)
{
    u16 n = 0;
    u32 tick = 0;
    float dis = 0.f;
    float pitch,roll,yaw;
    float VBAT_ADC = 0.f;    
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);    //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
    uart_init(115200);
   OLED_Init();
    AD_Init();
    csb_Init();
    pwm_init();
    LED_Init();
    Encoder_Init();
    exit_init();
   
   while (1)
   {

        if(Get_KEY1_Flag() == 0)
        {
            while(Get_KEY1_Flag() == 0)
            {
                GUIRuning();
                carstop();
                //OLED_ShowString(1,1,"STOP!!        ");
            }

        }       
        else
        {
            //carrunOK();
            OLED_ShowString(1,1,"AE86 RUNING!!");
        }
        

        if(GetStopFlag() == 0)
        {
            pid_runing();   
        }

        

        dis = csb_distance();
        OLED_ShowString(2,1,"Dis:");
        OLED_ShowNum(2,5,(int)dis,3);
        if(dis<50)
        {
            BEEP_Blink(); 
            //BEEP_Blink(); 
            if(GetAvoidanceFlag() == 1)
            {
                LED0 = !LED0;
                Avoidance();
            }        
            else                
            {
                carstop();
            }
            
        }
        else
        {
            carrunOK();
        }
        
        
        if (RATE_DO_EXECUTE(RATE_50_HZ, tick))
      {
            VBAT_ADC = AD_GetValue();
            OLED_ShowString(2,10,"V:");
            OLED_ShowNum(2,12,(int)VBAT_ADC,2);
      }
        tick++;
   }
}

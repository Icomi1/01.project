#include "GUI.h"
#include "OLED.h"
#include "led.h"
#include "Delay.h"
#include "key.h"
#include "pwm.h"


static u8 LineNum = 1;
u8 AvoidanceFlag = 0;

void GUIRuning(void)
{
    OLED_Clear();
    OLED_ShowString(LineNum,1,"@");
    OLED_ShowString(1,3,"Runing   ");
    OLED_ShowString(2,3,"Avoidance");
    OLED_ShowString(3,3,"NotAvoidance");
    OLED_ShowString(4,3,"Information");
    if(KEY2 == 0)
    {
        delay_ms(20);
      while (KEY2 == 0);
      delay_ms(20);
        LableUp();
        LED0 = !LED0;
    }
    if(KEY3 == 0)
    {
        delay_ms(20);
      while (KEY3 == 0);
      delay_ms(20);
        LableDowm();
        LED0 = !LED0;
    }
    if(KEY4 == 0)
    {
        delay_ms(20);
      while (KEY4 == 0);
      delay_ms(20);
        LableEnter(LineNum);
        LED0 = !LED0;
    }
}


void LableUp(void)
{
    if(LineNum==1)
        LineNum = 4;
    else
        LineNum--;
}


void LableDowm(void)
{
    if(LineNum==4)
        LineNum = 1;
    else 
        LineNum++;
}


void LableEnter(u8 num)
{
    if(num == 1)
    {
        carrunOK();
    }
    else if(num == 2)
    {
        AvoidanceFlag = 1;
    }
    else if(num == 3)
    {
        AvoidanceFlag = 0;
    }
    else if(num == 4)
    {
        OLED_Clear();
        while(1)
        {
            OLED_ShowString(1,1,"2240707321");
            OLED_ShowString(2,1,"ZhongZhuoLiang");
            OLED_ShowString(3,1,"VisionCar");
            if(KEY1==0|KEY2==0|KEY3==0|KEY4==0)
            {
                delay_ms(200);
                break;
            }
            
        }
    }
}


u8 GetAvoidanceFlag(void)
{
    return (AvoidanceFlag);
}

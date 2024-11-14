#include "led.h" 
#include "Delay.h"

//初始化PC13输出口.并使能这两个口的时钟          
//LED IO初始化
void LED_Init(void)
{        
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_ResetBits(GPIOA,GPIO_Pin_5);//设置高，灯灭
    GPIO_ResetBits(GPIOC,GPIO_Pin_13);//设置高，灯灭c
}


//LED闪烁函数
void LED_Blink(void)
{
    LED0 = 0;delay_ms(500);
    LED0 = 1;delay_ms(500);
}

void BEEP_Blink(void)
{
    BEEP = 1;delay_ms(200);
    BEEP = 0;delay_ms(200);
}

void BEEP_Hint(void)
{
    BEEP = 1;delay_ms(50);
    BEEP = 0;delay_ms(50);
}


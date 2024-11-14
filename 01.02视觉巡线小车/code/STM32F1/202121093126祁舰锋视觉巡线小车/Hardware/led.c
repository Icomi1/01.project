#include "led.h" 
#include "Delay.h"

//��ʼ��PC13�����.��ʹ���������ڵ�ʱ��          
//LED IO��ʼ��
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

    GPIO_ResetBits(GPIOA,GPIO_Pin_5);//���øߣ�����
    GPIO_ResetBits(GPIOC,GPIO_Pin_13);//���øߣ�����c
}


//LED��˸����
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


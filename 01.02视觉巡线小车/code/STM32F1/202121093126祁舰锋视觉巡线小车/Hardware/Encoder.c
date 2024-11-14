#include "Encoder.h"
#include "usart.h"

void Encoder_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3, ENABLE);//使能TIM3、TIM2时钟  
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能GPIOA时钟

    GPIO_StructInit(&GPIO_InitStructure);//将GPIO_InitStruct中的参数按缺省值输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_6 | GPIO_Pin_7;         
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//PA6 PA7浮空输入  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);     
    
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Period = 1560-1;  //设定计数器重装值   TIMx_ARR = 1024*4-1 这是360线的
    TIM_TimeBaseStructure.TIM_Prescaler = 0; //TIM4时钟预分频值
    TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1 ;//设置时钟分割 T_dts = T_ck_int    
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数     
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);     

    TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
    TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Falling, TIM_ICPolarity_Rising);
 
   TIM_ICStructInit(&TIM_ICInitStructure);
   TIM_ICInitStructure.TIM_Channel = TIM_Channel_1|TIM_Channel_2;
   TIM_ICInitStructure.TIM_ICFilter = 0xF;
   TIM_ICInit(TIM2, &TIM_ICInitStructure);
    
    TIM_ICStructInit(&TIM_ICInitStructure);
   TIM_ICInitStructure.TIM_Channel = TIM_Channel_1|TIM_Channel_2;
   TIM_ICInitStructure.TIM_ICFilter = 0xF;
   TIM_ICInit(TIM3, &TIM_ICInitStructure);
 
    TIM_Cmd(TIM2, ENABLE); 
    TIM_Cmd(TIM3, ENABLE); 
}

void GetSpeed(float* speed1,float* speed2)
{
    *speed1 = (float)TIM_GetCounter(TIM2);
    
    *speed2 = (float)TIM_GetCounter(TIM3);
    //printf("%.2f\r\n",*speed2);
    TIM2->CNT = 0;
    TIM3->CNT = 0;
}

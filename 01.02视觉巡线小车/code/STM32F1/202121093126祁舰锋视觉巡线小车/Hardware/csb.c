#include "csb.h"
#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "stm32f10x_tim.h"


void csb_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO_InitStructure);

    TIM_InternalClockConfig(TIM4); 

    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period=65535;
    TIM_TimeBaseInitStructure.TIM_Prescaler=71;
    TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);
    
    TIM_ICInitTypeDef TIM_ICInitStructure;
    TIM_ICInitStructure.TIM_Channel=TIM_Channel_1;
    TIM_ICInitStructure.TIM_ICFilter=0xf;
    TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising;
    TIM_ICInitStructure.TIM_ICPrescaler=TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI;
    TIM_PWMIConfig(TIM4,&TIM_ICInitStructure);

    TIM_SelectInputTrigger(TIM4,TIM_TS_TI1FP1);
    TIM_SelectSlaveMode(TIM4,TIM_SlaveMode_Reset);
    
    TIM_Cmd(TIM4,ENABLE);
}

void csb_Init1(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_TIM1,ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);

    TIM_InternalClockConfig(TIM1); 

    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period=65535;
    TIM_TimeBaseInitStructure.TIM_Prescaler=71;
    TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);
    
    TIM_ICInitTypeDef TIM_ICInitStructure;
    TIM_ICInitStructure.TIM_Channel=TIM_Channel_1;
    TIM_ICInitStructure.TIM_ICFilter=0xf;
    TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising;
    TIM_ICInitStructure.TIM_ICPrescaler=TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI;
    TIM_PWMIConfig(TIM1,&TIM_ICInitStructure);

    TIM_SelectInputTrigger(TIM1,TIM_TS_TI1FP1);
    TIM_SelectSlaveMode(TIM1,TIM_SlaveMode_Reset);
    
    TIM_Cmd(TIM1,ENABLE);
}


float distance;
int ccr;
float csb_distance(void)
{
    GPIO_ResetBits(GPIOB, GPIO_Pin_7);//预先拉低Trig引脚
    GPIO_SetBits(GPIOB, GPIO_Pin_7);
    Delay_us(11);
    GPIO_ResetBits(GPIOB, GPIO_Pin_7);//将Trig拉高20us，发射条件：Trig拉高10us以上
    ccr=TIM_GetCapture2(TIM4);//读取TIM4的CCR2的值，即高电平时长
    distance=(float)ccr/58;
    return distance;
    
//    GPIO_ResetBits(GPIOA, GPIO_Pin_11);//预先拉低Trig引脚
//    GPIO_SetBits(GPIOA, GPIO_Pin_11);
//    Delay_us(11);
//    GPIO_ResetBits(GPIOA, GPIO_Pin_11);//将Trig拉高20us，发射条件：Trig拉高10us以上
//    ccr=TIM_GetCapture2(TIM1);//读取TIM4的CCR2的值，即高电平时长
//    distance=(float)ccr/58;
//    return distance;
}


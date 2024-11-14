#include "stm32f10x.h"                  // Device header
#include "Timer.h"     
//extern uint16_t Num;
//初始化步骤：
//1.RCC开启时钟 2.选择时基单元的时钟源 
//3.配置时基单元 4.配置输出中断控制 5.置NVIC 6.运行控制
void Timer_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	TIM_InternalClockConfig(TIM2);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period=10000 - 1;//定时时间=(PSC+1)*(ARR+1)/72MHz多加了0
	TIM_TimeBaseInitStructure.TIM_Prescaler=720-1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);
	
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM2,ENABLE);
}

/*
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)== SET)
	{
		
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}	
	
}
*/
/* 定时器中断函数，可以复制到使用它的地方
void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
	{
		
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}
*/

#include "stm32f10x.h"                  // Device header

//1.RCC开启时钟 2.配置时基单元 3.配置输出比较单元 4.配置GPIO 5.运行控制
void PWM_Init(void)
{
	//RCC开启时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//开启TIM3时钟
	TIM_InternalClockConfig(TIM3);
	//配置GPIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	//配置时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period=20000-1;//频率=72MHz/(PSC+1)/(ARR+1)//ARR  周期：1/频率=PSC+1)*(ARR+1)/72MHz
	TIM_TimeBaseInitStructure.TIM_Prescaler=72-1;//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	//配置输出比较单元
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse=0;//CCR 500-2500 对应0.5ms到2.5ms
	TIM_OC1Init(TIM3,&TIM_OCInitStructure);
	TIM_OC2Init(TIM3,&TIM_OCInitStructure);
	TIM_OC3Init(TIM3,&TIM_OCInitStructure);
	TIM_OC4Init(TIM3,&TIM_OCInitStructure);
	
	TIM_Cmd(TIM3,ENABLE);//使能TIM3
}
/**
  * 函    数：PWM设置CCR
  * 参    数：Compare 要写入的CCR的值，范围：
  * 返 回 值：无
  * 注意事项：CCR和ARR共同决定占空比，此函数仅设置CCR的值，并不直接是占空比
  *           占空比Duty = CCR / (ARR + 1)
  */
void PWM_SetCompare1(uint16_t Compare)
{
	TIM_SetCompare1(TIM3,Compare);//设置CCR1的值
}

void PWM_SetCompare2(uint16_t Compare)
{
	TIM_SetCompare2(TIM3,Compare);//设置CCR2的值
}
void PWM_SetCompare3(uint16_t Compare)
{
	TIM_SetCompare3(TIM3,Compare);//设置CCR3的值
}
void PWM_SetCompare4(uint16_t Compare)
{
	TIM_SetCompare4(TIM3,Compare);//设置CCR4的值
}

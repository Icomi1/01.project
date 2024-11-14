#include "stm32f10x.h"                  // Device header
#include "Timer.h"
#include "time.h"
#include "stdlib.h"
#include "stdio.h"
#include "SuiJiShu.h"
#include "sys.h"
#include "time.h"
#include "usart2.h"
#include "DS1302.h"
#include "string.h"
#include "LED.h"
extern u8 TIMELED1_S_NUM;
extern u8 TIMELED1_M_NUM;
extern u8 TIMELED1_H_NUM;
u8 TIMELED1_S_NUM_JS;
u8 TIMELED1_M_NUM_JS;
u8 TIMELED1_H_NUM_JS;
int TIMELED1_SMH;
unsigned long long NUMLED1_JS;
unsigned long long NUMLED2_JS;
unsigned long long NUMLED3_JS;
unsigned long long NUMLED4_JS;
extern char TX[100];
extern int GUI1_LED;
extern int GUI2_LED;
extern int GUI3_LED;
extern int GUI4_LED;
extern char TIMELED1_S[100];
extern char TIMELED1_M[100];
extern char TIMELED1_H[100];
unsigned long long Num=0;
extern uint64_t sjh;
extern uint32_t SJS;
extern uint32_t SJSBY;
void SuiJiShu(void)
{
	srand(Num*1000);
	SJS=rand()%10000+1;//%取余
	
}
void SuiJiShuBY(void)//随机数备用
{
	srand(Num*1000);
	SJSBY=rand()%1000000+1;
}
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)== SET)
	{
		Num++;
		if(Num>=10000000000)
		{
			Num=0;
		}
		if(GUI1_LED==1)/////TIM2_IRQHandler,系统初始化之后，就每隔100ms进来里面的函数，
		{					////系统初始化之后，Num就每100ms加1
							////Num=10到Num=11，100ms
							////
			LED1_ON();
			GUI1_LED=0;
			NUMLED1_JS=Num;
//			sprintf(TX,"%lld",NUMLED1_JS);
//			UART3SendString((u8*)TX,strlen((char*)TX));
			if(NUMLED1_JS>=10000000000)
			{
				NUMLED1_JS=NUMLED1_JS-10000000000;
			}
		}
		if((Num-NUMLED1_JS)==200)
		{
			LED1_OFF();
			
		}
		if(GUI2_LED==1)
		{
			LED2_ON();
			GUI2_LED=0;
			NUMLED2_JS=Num;
//			sprintf(TX,"%lld",NUMLED2_JS);
//			UART3SendString((u8*)TX,strlen((char*)TX));
			if(NUMLED2_JS>=10000000000)
			{
				NUMLED2_JS=NUMLED2_JS-10000000000;
			}
		}
		if((Num-NUMLED2_JS)==200)
		{
			LED2_OFF();
		}
		if(GUI3_LED==1&&GUI3_LED!=0)
		{
			LED3_ON();
			GUI3_LED=0;
			
			NUMLED3_JS=Num;
//			sprintf(TX,"%lld",NUMLED3_JS);
//			UART3SendString((u8*)TX,strlen((char*)TX));
			if(NUMLED3_JS>=10000000000)
			{
				NUMLED3_JS=NUMLED3_JS-10000000000;
			}
		}
		if((Num-NUMLED3_JS)==200)
		{
			LED3_OFF();
		}
		if(GUI4_LED==1)
		{
			LED4_ON();
			GUI4_LED=0;
			NUMLED4_JS=Num;
//			sprintf(TX,"%lld",NUMLED4_JS);
//			UART3SendString((u8*)TX,strlen((char*)TX));
			if(NUMLED4_JS>=10000000000)
			{
				NUMLED4_JS=NUMLED4_JS-10000000000;
			}
		}
		if((Num-NUMLED4_JS)==200)
		{
			LED4_OFF();
		}

		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}	
	
}

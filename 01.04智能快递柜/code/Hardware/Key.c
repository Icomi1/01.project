#include "stm32f10x.h"                  // Device header
#include "delay.h"  
#include "Key.h"  
void Key1_Init(void)
{
	/*开启时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		//开启GPIOA的时钟
	
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1| GPIO_Pin_5|GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
}
void Key2_Init(void)
{
	/*开启时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);		//开启GPIOA的时钟
	
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
}
void Key_Init(void)
{
	Key1_Init();
	Key2_Init();
}
uint8_t Key_GetNum(void)//返回值函数
{
	uint8_t KeyNum=0;//先初始化变量
	if (GPIO_ReadInputDataBit(KeyProt1,Key0Kou) ==0)//读取PB1口输入状态，及有没有按下
	{
		delay_ms(20);//延时消抖
		while(GPIO_ReadInputDataBit(KeyProt1,Key0Kou) ==0);//检测松手，只有松开才执行下面的程序
		delay_ms(20);//松手抖动
		KeyNum=1;//为变量赋值
	}
	if (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1) ==0)
	{
		delay_ms(20);
		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1) ==0);
		delay_ms(20);
		KeyNum=2;
	}
	if (GPIO_ReadInputDataBit(KeyProt2,Key2Kou) ==0)
	{
		delay_ms(20);
		while(GPIO_ReadInputDataBit(KeyProt2,Key2Kou) ==0);
		delay_ms(20);
		KeyNum=3;
	}
	if (GPIO_ReadInputDataBit(KeyProt1,Key3Kou) ==0)
	{
		delay_ms(20);
		while(GPIO_ReadInputDataBit(KeyProt1,Key3Kou) ==0);
		delay_ms(20);
		KeyNum=4;
	}
	if (GPIO_ReadInputDataBit(KeyProt1,Key4Kou) ==0)
	{
		delay_ms(20);
		while(GPIO_ReadInputDataBit(KeyProt1,Key4Kou) ==0);
		delay_ms(20);
		KeyNum=5;
	}
	return (KeyNum);

}

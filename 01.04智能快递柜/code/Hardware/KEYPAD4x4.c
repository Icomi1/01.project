#include "stm32f10x.h"
#include "KEYPAD4x4.h"
#include "delay.h"
//	 

//********************************************************************************
u8 FLAG = 0;//进入扫描标志

void KEY_4x4_Init(void) //初始化
{
 	GPIO_InitTypeDef GPIO_InitStructure;
	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//开启时钟
	GPIO_InitStructure.GPIO_Pin  = KEYa|KEYb|KEYc|KEYd;  //行  PB12-PB15
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
	GPIO_Init(KEY_PROTH, &GPIO_InitStructure);
	//将PB12-PB15初始化为推挽输出模式
	GPIO_SetBits(KEY_PROTH,KEYa|KEYb|KEYc|KEYd);//将行都置为高电平
	
	//init GPIOA	  上拉输入
	GPIO_InitStructure.GPIO_Pin  = KEY1|KEY2|KEY3|KEY4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;// 列PA8-PA11
	GPIO_Init(KEY_PROTL, &GPIO_InitStructure);
	//将PA8-PA11初始化为上拉输入模式
	GPIO_SetBits(KEY_PROTL,KEY1|KEY2|KEY3|KEY4);//将列都置为高电平
		 

}
void KEY_Scan(int *key) //按键扫描函数
{	 
	GPIO_ResetBits(KEY_PROTH,KEYa);//将第一行置低电平
	GPIO_SetBits(KEY_PROTH,KEYb|KEYc|KEYd);
	GPIO_SetBits(KEY_PROTL,KEY1|KEY2|KEY3|KEY4);
	if((KEY1_Input==RESET)||(KEY2_Input==RESET)||(KEY3_Input==RESET)||(KEY4_Input==RESET))   
	{
		delay_ms(20);//去抖动 
		if(KEY1_Input==RESET)
		{
			while(!GPIO_ReadInputDataBit(KEY_PROTL,KEY1));//等待按键释放
			delay_ms(20);
			FLAG = 1;
			*key = 1;//将1-1（第一行第一列）设为1	
		}
		else if(KEY2_Input==RESET)
		{
	     	while(!GPIO_ReadInputDataBit(KEY_PROTL,KEY2));
			delay_ms(20);
			FLAG = 1;
			*key = 2;//将1-2设为2				
		}
		else if(KEY3_Input==RESET)
		{
			while(!GPIO_ReadInputDataBit(KEY_PROTL,KEY3));
			delay_ms(20);
	    	FLAG = 1;
			*key = 3;//将1-3设为3		
		}
		else if(KEY4_Input==RESET)
		{
			while(!GPIO_ReadInputDataBit(KEY_PROTL,KEY4));
			delay_ms(20);
	    	FLAG = 1;
			*key = 13;//将1-4设为13	
		}
		else 
		{
			FLAG = 0;
			GPIO_SetBits(KEY_PROTH,KEYa|KEYb|KEYc|KEYd);//回复原状，都置高电平
			GPIO_SetBits(KEY_PROTL,KEY1|KEY2|KEY3|KEY4);
		}
	}
	GPIO_ResetBits(KEY_PROTH,KEYb);////将第二行置低电平
	GPIO_SetBits(KEY_PROTH,KEYa|KEYc|KEYd);
	GPIO_SetBits(KEY_PROTL,KEY1|KEY2|KEY3|KEY4);
	if((KEY1_Input==RESET)||(KEY2_Input==RESET)||(KEY3_Input==RESET)||(KEY4_Input==RESET))
	{
		delay_ms(20);//去抖动 
		
		if(KEY1_Input==RESET)
		{
			while(!GPIO_ReadInputDataBit(KEY_PROTL,KEY1));
			delay_ms(20);
			FLAG = 1;
			*key = 4;
				
		}
		else if(KEY2_Input==RESET)
		{
			while(!GPIO_ReadInputDataBit(KEY_PROTL,KEY2));
			delay_ms(20);
	     	FLAG = 1;
			*key = 5;
		}
		else if(KEY3_Input==RESET)
		{
			while(!GPIO_ReadInputDataBit(KEY_PROTL,KEY3));
			delay_ms(20);
	    	FLAG = 1;
				*key = 6;
				
		}
		else if(KEY4_Input==RESET)
		{
			while(!GPIO_ReadInputDataBit(KEY_PROTL,KEY4));
			delay_ms(20);
	    	FLAG = 1;
				*key = 14;
			
		}
		else 
		{
			FLAG = 0;
			GPIO_SetBits(KEY_PROTH,KEYa|KEYb|KEYc|KEYd);
			GPIO_SetBits(KEY_PROTL,KEY1|KEY2|KEY3|KEY4);
		}
	}
	GPIO_ResetBits(KEY_PROTH,KEYc);//将第三行置低电平
	GPIO_SetBits(KEY_PROTH,KEYb|KEYa|KEYd);
	GPIO_SetBits(KEY_PROTL,KEY1|KEY2|KEY3|KEY4);
	if((KEY1_Input==RESET)||(KEY2_Input==RESET)||(KEY3_Input==RESET)||(KEY4_Input==RESET))
	{
		delay_ms(20);//去抖动 
		
		if(KEY1_Input==RESET)
		{
			while(!GPIO_ReadInputDataBit(KEY_PROTL,KEY1));	
			delay_ms(20);
			FLAG = 1;
				*key = 7;
			
		}
		else if(KEY2_Input==RESET)
		{
			while(!GPIO_ReadInputDataBit(KEY_PROTL,KEY2));
			delay_ms(20);
	     	FLAG = 1;
				*key = 8;
				
		}
		else if(KEY3_Input==RESET)
		{
			while(!GPIO_ReadInputDataBit(KEY_PROTL,KEY3));
			delay_ms(20);
	    	FLAG = 1;
			*key = 9;
		}
		else if(KEY4_Input==RESET)
		{
			while(!GPIO_ReadInputDataBit(KEY_PROTL,KEY4));
			delay_ms(20);
	    	FLAG = 1;
			*key = 15;
			
		}
		else 
		{
			FLAG = 0;
			GPIO_SetBits(KEY_PROTH,KEYa|KEYb|KEYc|KEYd);
			GPIO_SetBits(KEY_PROTL,KEY1|KEY2|KEY3|KEY4);
		}
	}
	GPIO_ResetBits(KEY_PROTH,KEYd);//将第四行置低电平
	GPIO_SetBits(KEY_PROTH,KEYb|KEYc|KEYa);
	GPIO_SetBits(KEY_PROTL,KEY1|KEY2|KEY3|KEY4);
	if((KEY1_Input==RESET)||(KEY2_Input==RESET)||(KEY3_Input==RESET)||(KEY4_Input==RESET))
	{
		delay_ms(20);//去抖动 
		
		if(KEY1_Input==RESET)
		{
			while(!GPIO_ReadInputDataBit(KEY_PROTL,KEY1));
			delay_ms(20);
			FLAG = 1;
			*key = 11;
			
		}
		else if(KEY2_Input==RESET)
		{
			while(!GPIO_ReadInputDataBit(KEY_PROTL,KEY2));
			delay_ms(20);
	     	FLAG = 1;
			*key = 10;
			
		}
		else if(KEY3_Input==RESET)
		{
			while(!GPIO_ReadInputDataBit(KEY_PROTL,KEY3));
			delay_ms(20);
	    	FLAG = 1;
				*key = 12;
				
		}
		else if(KEY4_Input==RESET)
		{
	 
			while(!GPIO_ReadInputDataBit(KEY_PROTL,KEY4));
			delay_ms(20);
	    	FLAG = 1;
			*key = 16;
				
		}
		else 
		{
			FLAG = 0;
			GPIO_SetBits(KEY_PROTH,KEYa|KEYb|KEYc|KEYd);
			GPIO_SetBits(KEY_PROTL,KEY1|KEY2|KEY3|KEY4);
		}
	}
	
}


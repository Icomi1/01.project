#ifndef __KEYPAD4X4_H
#define __KEYPAD4X4_H	 
//	 

//********************************************************************************
//V1.1修改说明
//修改按键扫描函数，使整个代码可以支持SWD下载。
//	 
#include "stm32f10x.h"

//#define KEY_HANG 0x000f   //0123
//#define KEY1 GPIO_Pin_4   
//#define KEY2 GPIO_Pin_5	   
//#define KEY3 GPIO_Pin_6	 
//#define KEY4 GPIO_Pin_7  
/*行*/
#define KEYa GPIO_Pin_12   
#define KEYb GPIO_Pin_13	   
#define KEYc GPIO_Pin_14	 
#define KEYd GPIO_Pin_15 
/*列*/
#define KEY1 GPIO_Pin_8   
#define KEY2 GPIO_Pin_9	   
#define KEY3 GPIO_Pin_10	 
#define KEY4 GPIO_Pin_11 

#define KEY1_Input GPIO_ReadInputDataBit(KEY_PROTL,KEY1)
#define KEY2_Input GPIO_ReadInputDataBit(KEY_PROTL,KEY2)
#define KEY3_Input GPIO_ReadInputDataBit(KEY_PROTL,KEY3)
#define KEY4_Input GPIO_ReadInputDataBit(KEY_PROTL,KEY4)

#define KEY_PROTH GPIOB
#define KEY_PROTL GPIOA

extern u8 FLAG;

void KEY_4x4_Init(void);//初始化
	
void KEY_Scan(int *key);//按键扫描函数	
#endif

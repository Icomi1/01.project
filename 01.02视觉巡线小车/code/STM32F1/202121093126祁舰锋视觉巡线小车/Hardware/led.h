#ifndef __LED_H
#define __LED_H
#include "stm32f10x.h"                  // Device header
#include "sys.h"

//LED端口定义
#define LED0 PAout(5)   // D2
#define BEEP PCout(13)   // D2


void LED_Init(void);//初始化      
void LED_Blink(void);//LED闪烁函数
void BEEP_Blink(void);
void BEEP_Hint(void);


#endif

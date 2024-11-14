#ifndef __LED_H
#define __LED_H
#include "stm32f10x.h"                  // Device header
#include "sys.h"

//LED�˿ڶ���
#define LED0 PAout(5)   // D2
#define BEEP PCout(13)   // D2


void LED_Init(void);//��ʼ��      
void LED_Blink(void);//LED��˸����
void BEEP_Blink(void);
void BEEP_Hint(void);


#endif

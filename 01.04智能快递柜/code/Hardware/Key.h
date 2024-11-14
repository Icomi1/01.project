#ifndef __KEY_H
#define __KEY_H
#include "sys.h"

#define KeyProt1 GPIOB
#define KeyProt2 GPIOA
#define Key0Kou GPIO_Pin_0
#define Key1Kou GPIO_Pin_1
#define Key2Kou GPIO_Pin_11
#define Key3Kou GPIO_Pin_4
#define Key4Kou GPIO_Pin_5
#define Key0 PBin(0); //LED KEY
#define Key1 PBin(1); // Buzzer KEY
#define Key2 PAin(11); //LED KEY
#define Key3 PBin(4); // Buzzer KEY
#define Key4 PBin(5); // Buzzer KEY
#define Key0read  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)//读取按键1
#define Key1read  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)//读取按键2
#define Key2read  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)//读取按键1
#define Key3read  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4)//读取按键2
#define Key4read  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)//读取按键2
void Key1_Init(void);
void Key2_Init(void);
void Key_Init(void);
uint8_t Key_GetNum(void);

#endif

#ifndef __LED_H
#define __LED_H

//#define LED1read  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//
//#define LED2read  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)//
//#define LED3read  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)//
//#define LED4read  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)//¶ÁÈ¡°´¼ü2
void LED_Init(void);
void LED1_ON(void);
void LED1_OFF(void);
void LED1_Turn(void);
void LED2_ON(void);
void LED2_OFF(void);
void LED2_Turn(void);
void LED3_ON(void);
void LED3_OFF(void);
void LED3_Turn(void);
void LED4_ON(void);
void LED4_OFF(void);
void LED4_Turn(void);

#endif

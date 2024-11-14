#ifndef __PWM_H
#define __PWM_H
#include "stm32f10x.h"                 // Device header

void pwm_init(void);
void SetMotor1(uint16_t speed1);
void SetMotor2(uint16_t speed1);
void SetDuoji1(uint16_t speed);
void SetDuoji2(uint16_t speed);
void carstop(void);
uint16_t GetStopFlag(void);
void carrunOK(void);
void CarBack(void);


#endif


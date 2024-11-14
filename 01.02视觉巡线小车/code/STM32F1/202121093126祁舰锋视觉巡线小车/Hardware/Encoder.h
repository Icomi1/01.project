#ifndef __ENCODER_H
#define __ENCODER_H
#include "stm32f10x.h"

void Encoder_Init(void);
void GetSpeed(float* speed1,float* speed2);

#endif

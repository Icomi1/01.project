#ifndef __BSP_H
#define __BSP_H

#include "stm32f1xx_hal.h"



void Rx_Display(void);
void Phy_Display(void);



void SG90_dispaly(TIM_HandleTypeDef * htim,uint32_t Channel, uint32_t Arr, short angle);

//����
void Advance(void);
void Retreat(void);
void Left(void);
void Right(void);
void Stand(void);
void Shake(void);
void Hello(void);//����
void Get_down1(void);//��ɢſ��
void Sit_down(void);//����


void Talk(void);//����
void Read_Distance(void);

void Exercise(void);
void Sleep_Display(void);


void OLED_Display(void);
void Face_proc(void);

void LED_Display(void);
void Sy_proc(void);



extern uint8_t rxdat;
extern uint8_t rxdat2;

extern uint16_t sec1_time;
extern uint8_t sys_time[3];
#endif


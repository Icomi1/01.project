#ifndef __KEY_H
#define __KEY_H
#include "stm32f10x.h"                  // Device header
#include "sys.h"

//LED¶Ë¿Ú¶¨Òå
#define KEY1 PCin(14)   // D2
#define KEY2 PCin(15)   // D2
#define KEY3 PBin(14)   // D2
#define KEY4 PBin(15)   // D2

void key_init(void);
void exit_init(void);
u8 Get_KEY1_Flag(void);



#endif

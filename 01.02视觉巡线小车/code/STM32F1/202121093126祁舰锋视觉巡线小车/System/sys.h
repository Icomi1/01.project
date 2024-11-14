#ifndef __SYS_H
#define __SYS_H   
#include "stm32f10x.h"

//0,不支持ucos
//1,支持ucos
#define SYSTEM_SUPPORT_UCOS      0      //定义系统文件夹是否支持UCOS
                                                       
    
//位带操作,实现51类似的GPIO控制功能
//具体实现思想,参考<<CM3权威指南>>第五章(87页~92页).
//IO口操作宏定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO口地址映射
#define GPIOA_ODR_Addr    0x4001080C   //(GPIOA_BASE+12)
#define GPIOB_ODR_Addr    0x40010C0C   //(GPIOB_BASE+12)
#define GPIOC_ODR_Addr    0x4001100C   //(GPIOC_BASE+12)
#define GPIOD_ODR_Addr    0x4001140C   //(GPIOD_BASE+12)
#define GPIOE_ODR_Addr    0x4001180C   //(GPIOE_BASE+12)
#define GPIOF_ODR_Addr    0x40011A0C   //(GPIOF_BASE+12)    
#define GPIOG_ODR_Addr    0x40011E0C   //(GPIOG_BASE+12)    

#define GPIOA_IDR_Addr   0x40010808  // (GPIOA_BASE+8)
#define GPIOB_IDR_Addr   0x40010C08  // (GPIOB_BASE+8)
#define GPIOC_IDR_Addr   0x40011008  // (GPIOC_BASE+8)
#define GPIOD_IDR_Addr   0x40011408  // (GPIOD_BASE+8)
#define GPIOE_IDR_Addr   0x40011808  // (GPIOE_BASE+8)
#define GPIOF_IDR_Addr   0x40011A08  // (GPIOF_BASE+8)
#define GPIOG_IDR_Addr   0x40011E08  // (GPIOG_BASE+8)
 
//IO口操作,只对单一的IO口!
//确保n的值小于16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

#define PBout(n)   BIT_ADDR(0x40010C0C,n)  //输出 
#define PBin(n)    BIT_ADDR(0x40010C08,n)  //输入 

#define PCout(n)   BIT_ADDR(0x4001100C,n)  //输出 
#define PCin(n)    BIT_ADDR(0x40011008,n)  //输入 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入



void NVIC_Configuration(void);



#endif

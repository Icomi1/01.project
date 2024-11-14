/**
  ******************************************************************************
  * @file    DHT11.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "DHT11.h"
#include "main.h"
#include "stm32f1xx_it.h"
#include "tim.h"


#define DHT11_DQ_OUT_LOW  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_14,GPIO_PIN_RESET)
#define DHT11_DQ_OUT_HIGH HAL_GPIO_WritePin(GPIOC,GPIO_PIN_14,GPIO_PIN_SET)
#define DHT11_DQ_IN    HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_14)

void delay_us(uint16_t us){
   uint16_t differ = 0xffff-us-5;            
   __HAL_TIM_SET_COUNTER(&htim3,differ);   //设定TIM2计数器起始值
   HAL_TIM_Base_Start(&htim3);      //启动定时器   
   
   while(differ < 0xffff-5){   //判断
      differ = __HAL_TIM_GET_COUNTER(&htim3);      //查询计数器的计数值
   }
   HAL_TIM_Base_Stop(&htim3);
}


void DHT11_IO_IN(void){      //IO口方向设置为输入
   GPIO_InitTypeDef GPIO_InitStructure;
   GPIO_InitStructure.Pin = GPIO_PIN_14;
   GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
   HAL_GPIO_Init(GPIOC,&GPIO_InitStructure);

}

void DHT11_IO_OUT(void){   //IO口方向设置为输出
   GPIO_InitTypeDef GPIO_InitStructure;
   GPIO_InitStructure.Pin = GPIO_PIN_14;
   GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
   GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
   HAL_GPIO_Init(GPIOC,&GPIO_InitStructure);
}

void DHT11_Rst(void){                 
   DHT11_IO_OUT();    //设置为输出
   DHT11_DQ_OUT_LOW;    //拉低
   HAL_Delay(20);       //至少18ms
   DHT11_DQ_OUT_HIGH;    //拉高 
   delay_us(30);        //至少20~40us
}

uint8_t DHT11_Check(void){   
   uint8_t retry=0;
   DHT11_IO_IN();      
   while (DHT11_DQ_IN&&retry<100){   //拉低40~80us
      retry++;
      delay_us(1);
   };    
   if(retry>=100)return 1;
   else retry=0;
   while (!DHT11_DQ_IN&&retry<100){ //拉高40~80us
      retry++;
      delay_us(1);
   };
   if(retry>=100)return 1;       
   return 0;   //检测到DHT11返回1
}

uint8_t DHT11_Read_Bit(void){
    uint8_t retry=0;
   while(DHT11_DQ_IN&&retry<100){   //等待变为低电平
      retry++;
      delay_us(1);
   }
   retry=0;
   while(!DHT11_DQ_IN&&retry<100){   //等待变为高电平
      retry++;
      delay_us(1);
   }
   delay_us(40);   //等待40us
   if(DHT11_DQ_IN)return 1;
   else return 0;         
}

uint8_t DHT11_Read_Byte(void){        
   uint8_t i,dat;
   dat=0;
   for (i=0;i<8;i++){
         dat<<=1; 
       dat|=DHT11_Read_Bit();
    }                      
    return dat;
}

uint8_t DHT11_Read_Data(uint16_t *temp,uint16_t *humi){        
    uint8_t buf[5];
   uint8_t i;
   DHT11_Rst();
   if(DHT11_Check()==0){
      for(i=0;i<5;i++){
         buf[i]=DHT11_Read_Byte();
      }
      if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4]){
         *humi=(buf[0]<<8) + buf[1];
         *temp=(buf[2]<<8) + buf[3];
      }
   }else return 1;
   return 0;       
}
      
uint8_t DHT11_Init(void){
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOA_CLK_ENABLE();         
    GPIO_Initure.Pin=GPIO_PIN_14;           
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  
    GPIO_Initure.Pull=GPIO_PULLUP;         
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);    
    DHT11_Rst();
   return DHT11_Check();
}

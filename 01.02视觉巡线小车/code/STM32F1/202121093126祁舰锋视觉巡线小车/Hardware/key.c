#include "key.h"
#include "Delay.h"
#include "led.h"

void key_init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);
   
   GPIO_InitTypeDef GPIO_InitStructure;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}   


void exit_init(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
   NVIC_InitTypeDef NVIC_InitStructure;
   
   key_init();
   
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE); 
   
   //配置GPIO引脚与外部中断控制线的映射关系
   GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource14);

   //中断配置结构体赋值------即选择中断触发的模式
   EXTI_InitStructure.EXTI_Line = EXTI_Line14;                              //lin3中断线
   EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;               //中断事件
   EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;         //下降沿触发
   EXTI_InitStructure.EXTI_LineCmd = ENABLE;                              //中断使能
   EXTI_Init(&EXTI_InitStructure); //根据 EXTI_InitStruct 中指定的
      
   //中断触发时指向的中断服务子程序
   NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;                      //使能按键外部中断通道
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;    //抢占优先级 2，
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;             //子优先级 2
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                      //使能外部中断通道
   NVIC_Init(&NVIC_InitStructure);                                              //中断优先级分组初始

}

u8 KEY_Flag = 0;
void EXTI15_10_IRQHandler(void)
{
   if(EXTI_GetITStatus(EXTI_Line14)!=RESET)//判断某个线上的中断是否发生
   {
      delay_ms(200);//防抖动
      if(KEY1==1)
      {
         KEY_Flag = !KEY_Flag;
            LED0 = !LED0;
      }
      EXTI_ClearITPendingBit(EXTI_Line14); //清除 LINE 上的中断标志位
   }
}


u8 Get_KEY1_Flag(void)
{
    return (KEY_Flag);
}


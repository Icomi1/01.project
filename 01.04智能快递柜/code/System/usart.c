//硬件驱动
#include "usart.h"
#include "delay.h"
#include "led.h"
//C库
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
//重定向C库函数printf到串口，重定向后可使用printf函数
int fputc(int ch,FILE *f)
{
    /* 发送一个字节数据到串口 */
    USART_SendData(USART1,(uint8_t) ch);
    while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
    return (ch);
}
//重定向C库函数scanf到串口,重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
    /* 等待串口输入数据 */
    while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
    return (int)USART_ReceiveData(USART1);
}
void MyUSART_Init()
{
    /* 定义GPIO、NVIC和USART初始化的结构体 */
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    /* 使能GPIO和USART的时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//重定义
    /* 将USART TX（B6）的GPIO设置为推挽复用模式 */
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO_InitStructure);
    /* 将USART RX（B7）的GPIO设置为浮空输入模式 */
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;
    GPIO_Init(GPIOB,&GPIO_InitStructure);
	 GPIO_PinRemapConfig(GPIO_Remap_USART1,ENABLE);//重定义
    /* 配置串口 */
    USART_InitStructure.USART_BaudRate=9600;                                        //波特率了设置为9600
    USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;   //不使用硬件流控制
    USART_InitStructure.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;                     //使能接收和发送
    USART_InitStructure.USART_Parity=USART_Parity_No;                               //不使用奇偶校验位
    USART_InitStructure.USART_StopBits=USART_StopBits_1;                            //1位停止位
    USART_InitStructure.USART_WordLength=USART_WordLength_8b;                       //字长设置为8位
    USART_Init(USART1, &USART_InitStructure);   

    /* Usart1 NVIC配置 */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);                 //设置NVIC中断分组2
    NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
    NVIC_Init(&NVIC_InitStructure);

    /*初始化串口，开启串口接收中断 */
    USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
    /* 使能串口1 */
    USART_Cmd(USART1,ENABLE);

}
/* USART1中断函数 */
void USART1_IRQHandler(void)
{
    uint8_t ucTemp;                                         //接收数据
    if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)
    {      
        ucTemp = USART_ReceiveData(USART1);
        USART_SendData(USART1,ucTemp);
//        if(ucTemp == 0x32)
//        {
//            LED1_ON();
//        }
//        if(ucTemp == 0x31)
//        {
//            LED1_OFF();
//        }
    }
}

/* 发送一个字节 */
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
    /* 发送一个字节数据到USART */
    USART_SendData(pUSARTx,ch);

    /* 等待发送数据寄存器为空 */
    while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);  
}
/* 发送字符串 */
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
    unsigned int k=0;
  do
  {
      Usart_SendByte( pUSARTx, *(str + k) );
      k++;
  } while(*(str + k)!='\0');

  /* 等待发送完成 */
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
  {}
}
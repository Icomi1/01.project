//Ӳ������
#include "usart.h"
#include "delay.h"
#include "led.h"
//C��
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
//�ض���C�⺯��printf�����ڣ��ض�����ʹ��printf����
int fputc(int ch,FILE *f)
{
    /* ����һ���ֽ����ݵ����� */
    USART_SendData(USART1,(uint8_t) ch);
    while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
    return (ch);
}
//�ض���C�⺯��scanf������,��д����ʹ��scanf��getchar�Ⱥ���
int fgetc(FILE *f)
{
    /* �ȴ������������� */
    while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
    return (int)USART_ReceiveData(USART1);
}
void MyUSART_Init()
{
    /* ����GPIO��NVIC��USART��ʼ���Ľṹ�� */
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    /* ʹ��GPIO��USART��ʱ�� */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//�ض���
    /* ��USART TX��B6����GPIO����Ϊ���츴��ģʽ */
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO_InitStructure);
    /* ��USART RX��B7����GPIO����Ϊ��������ģʽ */
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;
    GPIO_Init(GPIOB,&GPIO_InitStructure);
	 GPIO_PinRemapConfig(GPIO_Remap_USART1,ENABLE);//�ض���
    /* ���ô��� */
    USART_InitStructure.USART_BaudRate=9600;                                        //������������Ϊ9600
    USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;   //��ʹ��Ӳ��������
    USART_InitStructure.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;                     //ʹ�ܽ��պͷ���
    USART_InitStructure.USART_Parity=USART_Parity_No;                               //��ʹ����żУ��λ
    USART_InitStructure.USART_StopBits=USART_StopBits_1;                            //1λֹͣλ
    USART_InitStructure.USART_WordLength=USART_WordLength_8b;                       //�ֳ�����Ϊ8λ
    USART_Init(USART1, &USART_InitStructure);   

    /* Usart1 NVIC���� */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);                 //����NVIC�жϷ���2
    NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
    NVIC_Init(&NVIC_InitStructure);

    /*��ʼ�����ڣ��������ڽ����ж� */
    USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
    /* ʹ�ܴ���1 */
    USART_Cmd(USART1,ENABLE);

}
/* USART1�жϺ��� */
void USART1_IRQHandler(void)
{
    uint8_t ucTemp;                                         //��������
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

/* ����һ���ֽ� */
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
    /* ����һ���ֽ����ݵ�USART */
    USART_SendData(pUSARTx,ch);

    /* �ȴ��������ݼĴ���Ϊ�� */
    while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);  
}
/* �����ַ��� */
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
    unsigned int k=0;
  do
  {
      Usart_SendByte( pUSARTx, *(str + k) );
      k++;
  } while(*(str + k)!='\0');

  /* �ȴ�������� */
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
  {}
}
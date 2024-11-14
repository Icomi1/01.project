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
   
   //����GPIO�������ⲿ�жϿ����ߵ�ӳ���ϵ
   GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource14);

   //�ж����ýṹ�帳ֵ------��ѡ���жϴ�����ģʽ
   EXTI_InitStructure.EXTI_Line = EXTI_Line14;                              //lin3�ж���
   EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;               //�ж��¼�
   EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;         //�½��ش���
   EXTI_InitStructure.EXTI_LineCmd = ENABLE;                              //�ж�ʹ��
   EXTI_Init(&EXTI_InitStructure); //���� EXTI_InitStruct ��ָ����
      
   //�жϴ���ʱָ����жϷ����ӳ���
   NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;                      //ʹ�ܰ����ⲿ�ж�ͨ��
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;    //��ռ���ȼ� 2��
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;             //�����ȼ� 2
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                      //ʹ���ⲿ�ж�ͨ��
   NVIC_Init(&NVIC_InitStructure);                                              //�ж����ȼ������ʼ

}

u8 KEY_Flag = 0;
void EXTI15_10_IRQHandler(void)
{
   if(EXTI_GetITStatus(EXTI_Line14)!=RESET)//�ж�ĳ�����ϵ��ж��Ƿ���
   {
      delay_ms(200);//������
      if(KEY1==1)
      {
         KEY_Flag = !KEY_Flag;
            LED0 = !LED0;
      }
      EXTI_ClearITPendingBit(EXTI_Line14); //��� LINE �ϵ��жϱ�־λ
   }
}


u8 Get_KEY1_Flag(void)
{
    return (KEY_Flag);
}


#include "stm32f10x.h"                 // Device header
#include "pwm.h"
#include "usart.h"
#include "Delay.h"
#include "led.h"

void pwm_init(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_TIM1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
    
    //�����������תIO�ӿ�
   GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_12|GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_SetBits(GPIOB,GPIO_Pin_1);
    GPIO_ResetBits(GPIOB,GPIO_Pin_0);
    GPIO_SetBits(GPIOB,GPIO_Pin_12);
    GPIO_ResetBits(GPIOB,GPIO_Pin_13);
    
    //���PWM����ӿ�
    //GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//���JTAG���Զ˿ڣ�����SW���Խӿڣ�

   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
   GPIO_InitStructure.GPIO_Pin =GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOA, &GPIO_InitStructure); 

   //����ʱ����Ԫ����
   TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
   TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;//��ʱ������Ƶ
   TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���ģʽ
   TIM_TimeBaseInitStructure.TIM_Period = 20000 - 1;//ARR�Զ���װֵ
   TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;//PSCԤ��Ƶֵ
   TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;//����ARR����һ���ж� ֹͣ����
   TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);//��ʼ����Ԫ

   //����ȽϽṹ������
   TIM_OCInitTypeDef TIM_OCInitStructure;
   TIM_OCStructInit(&TIM_OCInitStructure);//��ȫ�ṹ����δ���ò���
   TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;   
   TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//ѡ��PWMģʽ1
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//����Ƚϼ���ѡ��
   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//���ʹ��
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    TIM_OC2Init(TIM1, &TIM_OCInitStructure);
    TIM_OC3Init(TIM1, &TIM_OCInitStructure);
    TIM_OC4Init(TIM1, &TIM_OCInitStructure);
    
   TIM_Cmd(TIM1, ENABLE);//������ʱ��1
    
    TIM_CtrlPWMOutputs(TIM1,ENABLE);  //ȷ����TIM1����PWM
    
    TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);//ʹ��CCR1�Զ���װ
    TIM_OC2PreloadConfig(TIM1,TIM_OCPreload_Enable);//ʹ��CCR2�Զ���װ
    TIM_OC3PreloadConfig(TIM1,TIM_OCPreload_Enable);//ʹ��CCR3�Զ���װ
    TIM_OC4PreloadConfig(TIM1,TIM_OCPreload_Enable);//ʹ��CCR4�Զ���װ

   TIM_ARRPreloadConfig(TIM1,ENABLE);//����Ԥװ��
    
    TIM_SetCompare3(TIM1,1500);
    TIM_SetCompare4(TIM1,1500);
}



void SetMotor1(uint16_t speed1)
{
    uint32_t speed;
    if(speed1 > 19000) speed = 19000;
    else if (speed1 <0) speed = 0;
    else speed=(uint32_t)speed1;//�����������޷�
    TIM_SetCompare1(TIM1,speed+8500);
}

void SetMotor2(uint16_t speed1)
{
    uint32_t speed;
    if(speed1 > 19000) speed = 19000;
    else if (speed1 <0) speed = 0;
    else speed=(uint32_t)speed1;//�����������޷�
    TIM_SetCompare2(TIM1,speed+8500);
}

void SetDuoji1(uint16_t speed)
{
    if(speed>2500) speed=2500;
    else if(speed<500) speed=500;
    TIM_SetCompare3(TIM1,speed);
}

void SetDuoji2(uint16_t speed)
{
    if(speed>2500) speed=2500;
    else if(speed<500) speed=500;
    TIM_SetCompare4(TIM1,speed);
}

uint16_t CarStopFlag = 0;
void carstop(void)
{
    CarStopFlag  = 1;
    TIM_SetCompare1(TIM1,0);
    TIM_SetCompare2(TIM1,0);
}

void carrunOK(void)
{
    CarStopFlag = 0;
}

uint16_t GetStopFlag(void)
{
    return CarStopFlag;
}

void CarBack(void)
{
    SetDuoji1(1500);
    GPIO_SetBits(GPIOB,GPIO_Pin_0);
    GPIO_ResetBits(GPIOB,GPIO_Pin_1);
    GPIO_SetBits(GPIOB,GPIO_Pin_13);
    GPIO_ResetBits(GPIOB,GPIO_Pin_12);
    SetMotor1(5000);SetMotor2(5000);
    BEEP = 1;
    delay_ms(2000);
    BEEP = 0;
    GPIO_SetBits(GPIOB,GPIO_Pin_1);
    GPIO_ResetBits(GPIOB,GPIO_Pin_0);
    GPIO_SetBits(GPIOB,GPIO_Pin_12);
    GPIO_ResetBits(GPIOB,GPIO_Pin_13);
}

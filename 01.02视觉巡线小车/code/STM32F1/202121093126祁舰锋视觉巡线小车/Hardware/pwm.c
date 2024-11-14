#include "stm32f10x.h"                 // Device header
#include "pwm.h"
#include "usart.h"
#include "Delay.h"
#include "led.h"

void pwm_init(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_TIM1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
    
    //电机控制正反转IO接口
   GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_12|GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_SetBits(GPIOB,GPIO_Pin_1);
    GPIO_ResetBits(GPIOB,GPIO_Pin_0);
    GPIO_SetBits(GPIOB,GPIO_Pin_12);
    GPIO_ResetBits(GPIOB,GPIO_Pin_13);
    
    //电机PWM输出接口
    //GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//解除JTAG调试端口（保留SW调试接口）

   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
   GPIO_InitStructure.GPIO_Pin =GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOA, &GPIO_InitStructure); 

   //配置时基单元参数
   TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
   TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;//定时器不分频
   TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数模式
   TIM_TimeBaseInitStructure.TIM_Period = 20000 - 1;//ARR自动重装值
   TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;//PSC预分频值
   TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;//到达ARR触发一次中断 停止计数
   TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);//初始化单元

   //输出比较结构体配置
   TIM_OCInitTypeDef TIM_OCInitStructure;
   TIM_OCStructInit(&TIM_OCInitStructure);//补全结构体中未配置参数
   TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;   
   TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//选择PWM模式1
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//输出比较极性选择
   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//输出使能
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    TIM_OC2Init(TIM1, &TIM_OCInitStructure);
    TIM_OC3Init(TIM1, &TIM_OCInitStructure);
    TIM_OC4Init(TIM1, &TIM_OCInitStructure);
    
   TIM_Cmd(TIM1, ENABLE);//开启定时器1
    
    TIM_CtrlPWMOutputs(TIM1,ENABLE);  //确定让TIM1输入PWM
    
    TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);//使能CCR1自动重装
    TIM_OC2PreloadConfig(TIM1,TIM_OCPreload_Enable);//使能CCR2自动重装
    TIM_OC3PreloadConfig(TIM1,TIM_OCPreload_Enable);//使能CCR3自动重装
    TIM_OC4PreloadConfig(TIM1,TIM_OCPreload_Enable);//使能CCR4自动重装

   TIM_ARRPreloadConfig(TIM1,ENABLE);//开启预装载
    
    TIM_SetCompare3(TIM1,1500);
    TIM_SetCompare4(TIM1,1500);
}



void SetMotor1(uint16_t speed1)
{
    uint32_t speed;
    if(speed1 > 19000) speed = 19000;
    else if (speed1 <0) speed = 0;
    else speed=(uint32_t)speed1;//电机最终输出限幅
    TIM_SetCompare1(TIM1,speed+8500);
}

void SetMotor2(uint16_t speed1)
{
    uint32_t speed;
    if(speed1 > 19000) speed = 19000;
    else if (speed1 <0) speed = 0;
    else speed=(uint32_t)speed1;//电机最终输出限幅
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

#include "stm32f10x.h"                  // Device header
#include "PWM.h"
#include <Servo.h>
void Servo_Init(void)//舵机初始化
{
	PWM_Init();
	Servo_SetAngle1(0);
	Servo_SetAngle2(0);
	Servo_SetAngle3(0);
	Servo_SetAngle4(0);
}
/**
  * 函    数：舵机设置角度
  * 参    数：Angle 要设置的舵机角度，范围：0~180
  * 返 回 值：无
  */
void Servo_SetAngle1(float Angle)
{
	PWM_SetCompare1(Angle /180*2000+500);
}

void Servo_SetAngle2(float Angle)
{
	PWM_SetCompare2(Angle /180*2000+500);
}
void Servo_SetAngle3(float Angle)
{
	PWM_SetCompare3(Angle /180*2000+500);
}

void Servo_SetAngle4(float Angle)
{
	PWM_SetCompare4(Angle /180*2000+500);
}

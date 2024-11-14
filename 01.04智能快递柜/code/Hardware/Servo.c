#include "stm32f10x.h"                  // Device header
#include "PWM.h"
#include <Servo.h>
void Servo_Init(void)//�����ʼ��
{
	PWM_Init();
	Servo_SetAngle1(0);
	Servo_SetAngle2(0);
	Servo_SetAngle3(0);
	Servo_SetAngle4(0);
}
/**
  * ��    ����������ýǶ�
  * ��    ����Angle Ҫ���õĶ���Ƕȣ���Χ��0~180
  * �� �� ֵ����
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

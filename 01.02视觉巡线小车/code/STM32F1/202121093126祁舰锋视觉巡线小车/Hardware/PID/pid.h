#ifndef __PID_H
#define __PID_H
#include "stm32f10x.h"                  // Device header


#define RATE_5_HZ      5
#define RATE_10_HZ      10
#define RATE_25_HZ      25
#define RATE_50_HZ      50
#define RATE_100_HZ      100
#define RATE_200_HZ    200
#define RATE_250_HZ    250
#define RATE_500_HZ    500
#define RATE_1000_HZ    1000

#define MAIN_LOOP_RATE    RATE_1000_HZ
#define MAIN_LOOP_DT   (u32)(1000/MAIN_LOOP_RATE)   /*单位ms*/

#define RATE_DO_EXECUTE(RATE_HZ, TICK)  ((TICK % (MAIN_LOOP_RATE / RATE_HZ)) == 0)


typedef struct
{
    /**************位置式**************/
   float desired;      //< 目标值
   float error;        //< 误差
   float prevError;    //< 上一个误差
   float integ;        //< 积分项
   float deriv;        //< 微分项
   float kp;           //< 比例系数
   float ki;           //< 积分系数
   float kd;           //< 微分系数
   float outP;         //< 比例项
   float outI;         //< 积分项
   float outD;         //< 微分项
   float iLimit;       //< 限幅
   float outputLimit;  //< 总输出限幅
   float dt;           //< 微分变量t
   float out;         //< 最终输出值
} PID_Struct;


void pid_runing(void);
void pid_SetParameter(void);
float pidUpdate(PID_Struct* pid, const float error);
void SetDesired(PID_Struct* pid, uint16_t Desired);
void MotorControl(float motor1, float motor2);



#endif

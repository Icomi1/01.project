#include "pid.h"
#include "usart.h"
#include "Encoder.h"
#include "pwm.h"
#include "OLED.h"

/*****************变量定义********************/
PID_Struct left_pid,right_pid;            //左右轮PID结构体
uint16_t des_speed1,des_speed2,duoji;
float real_speed1,real_speed2;

//PID算法函数（位置式）
float pidUpdate(PID_Struct* pid, const float error)
{
    //printf("error:%.2f\r\n",error);
   float output;

   pid->error = error;   
    
   pid->integ += pid->error * 0.005f;//积分项
   //printf("pid->integ:%.2f\r\n",pid->integ);
   //积分限幅
   if (pid->integ > pid->iLimit)
   {
      pid->integ = pid->iLimit;
   }
   else if (pid->integ < -pid->iLimit)
   {
      pid->integ = -pid->iLimit;
   }

   //pid->deriv = (pid->error - pid->prevError) / 0.005f;

   pid->outP = 2.8f * error;
   pid->outI = 1.8f * pid->integ;
   //pid->outD = pid->kd * pid->deriv;
    
    //printf("P:%.2f, I:%.2f\r\n",pid->outP,pid->outI);
   
    output = pid->outP + pid->outI  ;//pid->outD
   
   //输出限幅
   if (pid->outputLimit != 0)
   {
      if (output > pid->outputLimit)
         output = pid->outputLimit;
      else if (output < -pid->outputLimit)
         output = -pid->outputLimit;
   }
   
   pid->prevError = pid->error;

   pid->out = output;
    
   return output;
}



void pid_SetParameter(void)
{
    left_pid.error     = 0.f;
   left_pid.prevError = 0.f;
   left_pid.integ     = 0.f;
   left_pid.deriv     = 0.f;
   left_pid.desired = 0.f;
   left_pid.kp = 1.f;
   left_pid.ki = 0.f;
   left_pid.kd = 0.f;
   left_pid.iLimit = 1000.f;
   left_pid.outputLimit = 19000.f;
   left_pid.dt = 0.005f;
    
    right_pid.error     = 0.f;
   right_pid.prevError = 0.f;
   right_pid.integ     = 0.f;
   right_pid.deriv     = 0.f;
   right_pid.desired = 0.f;
   right_pid.kp = 1.f;
   right_pid.ki = 0.f;
   right_pid.kd = 0.f;
   right_pid.iLimit = 1000.f;
   right_pid.outputLimit = 19000.f;
   right_pid.dt = 0.005f;
}

void SetDesired(PID_Struct* pid, uint16_t Desired)
{
    pid->desired = (float)Desired;
    //printf("%.2f\r\n",pid->desired);
}

void MotorControl(float motor1, float motor2)
{
    SetMotor1(motor1);
    SetMotor2(motor2);
}   


void pid_runing(void)
{
    float motor1=0,motor2=0;
    GetSpeed(&real_speed1,&real_speed2);
    
    GetVisionMotor(&des_speed1,&des_speed2);
    SetDesired(&left_pid, des_speed1);
    SetDesired(&right_pid, des_speed2);
    motor1 = pidUpdate(&left_pid,left_pid.desired - real_speed1);
    motor2 = pidUpdate(&right_pid,right_pid.desired - real_speed2);
    printf("%.2f, %.2f\r\n",motor1,motor2);
    GetVisionDuoji(&duoji);
    OLED_ShowString(3,1,"L:");
    OLED_ShowString(3,9,"R:");
    OLED_ShowNum(3,3,motor1,5);
    OLED_ShowNum(3,11,motor2,5);
    SetDuoji1(duoji);
    MotorControl(motor1,motor2);
}


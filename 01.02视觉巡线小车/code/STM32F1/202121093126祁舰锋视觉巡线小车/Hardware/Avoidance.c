#include "Avoidance.h"
#include "pwm.h"
#include "Delay.h"
#include "csb.h"

float distanceleft[300]={0.f},distanceright[300]={0.f};
void Avoidance(void)
{
    SetDuoji1(1500);
    static u16 DistanceNum;
    static u16 angle;
    static u16 numleft = 0,numright = 0;
    carstop();
    for(angle=1500;angle>500;angle-=5)
    {
        numleft++;
        SetDuoji2(angle);
        distanceleft[numleft] = csb_distance();
        delay_ms(20);
    }
    SetDuoji2(1500);
    DistanceNum = SearchMax(distanceleft);
    if(distanceleft[DistanceNum] > 150)
    {
        SetDuoji1(2500);
        SetMotor1(5000);SetMotor2(1000);
        delay_ms(1000);
        SetDuoji1(500);
        SetMotor1(1000);SetMotor2(5000);
        delay_ms(1500);
        SetDuoji2(1500);
    }
    else
    {
        for(angle=1500;angle<2500;angle+=5)
        {
            numright++;
            SetDuoji2(angle);
            distanceright[numright] = csb_distance();
            delay_ms(10);
        }
        DistanceNum = SearchMax(distanceright);
        if(distanceright[DistanceNum] > 120)
        {
            SetDuoji1(500);
            SetMotor1(1000);SetMotor2(5000);
            delay_ms(1000);
            SetDuoji1(2500);
            SetMotor1(5000);SetMotor2(1000);
            delay_ms(1500);
            SetDuoji2(1500);
        }
        else
        {
            CarBack();
            
        }
    }
}

u16 SearchMax(float distance[300])
{
    float disMax=0;
    u16 disMaxNum = 0;
    for(int ii=1;ii<201;ii++)
    {
        if(distance[ii]>disMax)
        {
            disMax = distance[ii];
            disMaxNum = ii;
        } 
    }
    return (disMaxNum);
}


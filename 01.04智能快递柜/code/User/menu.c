#include "oled.h"
#include "led.h"
#include "delay.h"
#include "menu.h"
#include "key.h" 
#include "led.h"
#include "KEYPAD4x4.h"
#include <stdio.h>
#include <AIR780.h>
#include <Servo.h>
#include "SuiJiShu.h"
#include <ASCII_TO_Unicode.h>
#include "string.h"
#include "Timer.h"
#include "usart2.h"
#include "Phone_Num.h"
#include "time.h"
#include "stdlib.h"
#include "DS1302.h"
#include "usart.h"
extern char phone_NumOK[100];
//========================variable==========================//
// u16 ColorTab[5]={RED,GREEN,BLUE,YELLOW,BRED};//������ɫ����
extern char phone_Num[100];//���봦��֮ǰ
extern int KeyNum;
extern unsigned char  func_index;
extern unsigned char last_index;
extern uint32_t SJS;
extern uint32_t SJSBY;
extern int RES;
extern  uint16_t Num;
int MSGBZ=0;
char res=0;
u32 bufe[5];
int GUI1=0;//�����Ƿ�����־
int GUI2=0;//�����Ƿ�����־
int GUI3=0;//�����Ƿ�����־
int GUI4=0;//�����Ƿ�����־
int KEY=0;
int K=0;
extern int GUI1_LED;
extern int GUI2_LED;
extern int GUI3_LED;
extern int GUI4_LED;
char TIMELED1_S[100];
char TIMELED1_M[100];
char TIMELED1_H[100];
extern uint64_t QuJianMa1,QuJianMa2,QuJianMa3,QuJianMa4;
extern char QJM[10];//
extern char QJMUnicode[10];//
char TX[100];
//char phone_center[100]= "0891683110709105F0";//��������
extern char MSG[100];//����
extern char MSG1[100];//����
unsigned long long QuJianMa1PhoneNum,QuJianMa2PhoneNum,QuJianMa3PhoneNum,QuJianMa4PhoneNum;
extern unsigned char  func_index;
extern  unsigned char last_index;
extern char* PhoneCENNum;

void my_testA0(void)//��ҳ����ʾ�����գ����ڣ�ʱ��
{
	UART3SendString((u8*)"��ǰҳ��Ϊ��ҳ\r\n��ʾ�������ա����ڡ�ʱ��\r\n",
	strlen((char*)"��ǰҳ��Ϊ��ҳ\r\n��ʾ�������ա����ڡ�ʱ��\r\n"));
	OLED_Clear();
//	OLED_ShowString(0,56,"K13",OLED_6X8);//�ġ�
	OLED_ShowImage(0,48,16,16,DUIHAO);
	OLED_Update();
	while(1)
	{
		KEY_Scan(&KeyNum);
		DS1302_Dis();
		if(KeyNum==11)//ȷ��
		{
			return;
		}
		
	}

}
void my_testB1(void)//��һ��-1�����ܿ�ݹ����ϵͳҳ����ͷ����	
{
	UART3SendString((u8*)"��ǰҳ��Ϊ���ܿ�ݹ����ϵͳҳ��\r\n��ʾ��ȡ����ȡ�������Ա���\r\n",
	strlen((char*)"��ǰҳ��Ϊ���ܿ�ݹ����ϵͳҳ��\r\n��ʾ��ȡ����ȡ�������Ա���\r\n"));
	OLED_ShowChinese(0,16,"���ܿ�ݿ���ϵͳ");
	OLED_ShowChinese(16,32,"ȡ����ȡ��");
	OLED_ShowChinese(16,48,"���Ա���");
	OLED_ShowImage(0,32,16,16,JianTou);
	OLED_ShowString(0, 48, "  ",8);
	switch(GUI1)
	{
		case 1:OLED_DrawRectangle(62,1,13,11,1);
		default:OLED_DrawRectangle(62,1,13,11,0);
	}
	switch(GUI2)
	{
		case 1:OLED_DrawRectangle(80,1,13,11,1);
		default:OLED_DrawRectangle(80,1,13,11,0);
	}
	switch(GUI3)
	{
		case 1:OLED_DrawRectangle(98,3,11,9,1);
		default:OLED_DrawRectangle(98,3,11,9,0);
	}
	switch(GUI4)
	{
		case 1:OLED_DrawRectangle(114,3,11,9,1);
		default:OLED_DrawRectangle(114,3,11,9,0);
	}
	OLED_Update();
	while(1)
	{
		DS1302_Dis1();
		KEY_Scan(&KeyNum);
		if(KeyNum==11)//����
		{
			return;
		}
		if(KeyNum==16)//�˳�
		{
			return;
		}
		if(KeyNum==14)//����
		{
			return;
		}
	}
}
void my_testB2(void)//��һ��-2�����ܿ�ݹ����ϵͳҳ����ͷ����	
{
	UART3SendString((u8*)"��ǰҳ��Ϊ���ܿ�ݹ����ϵͳҳ��\r\n��ʾ��ȡ����ȡ�������Ա���\r\n",
	strlen((char*)"��ǰҳ��Ϊ���ܿ�ݹ����ϵͳҳ��\r\n��ʾ��ȡ����ȡ�������Ա���\r\n"));
	OLED_ShowChinese(0,16,"���ܿ�ݿ���ϵͳ");
	OLED_ShowChinese(16,32,"ȡ����ȡ��");
	OLED_ShowChinese(16,48,"���Ա���");
   OLED_ShowImage(0,48,16,16,JianTou);
	OLED_ShowString(0, 32, "  ",8);
	OLED_Update();
	while(1)
	{
		DS1302_Dis1();
		KEY_Scan(&KeyNum);
		if(KeyNum==11)
		{
			return;
		}
		if(KeyNum==16)
		{
			return;
		}
		if(KeyNum==14)
		{
			return;
		}
	}
}

void my_testC3(void)//ȡ����ȡ��
{
	UART3SendString((u8*)"��ǰҳ��Ϊȡ��������ȡ����ҳ��\r\n��ʾ��������ȡ����\r\n",
	strlen((char*)"��ǰҳ��Ϊȡ��������ȡ����ҳ��\r\n��ʾ��������ȡ����\r\n"));
	OLED_ShowChinese(0,0,"������ȡ����");
	OLED_ShowString(96,0,":",8);
	OLED_Update();
	KeyNum=0;
	int JD1=0,JD2=0,JD3=0,JD4=0;
	unsigned long long Sum=0; 
	int Show_Num=-8;
	while(1)
	{
		KEY_Scan(&KeyNum);
		if(KeyNum) 
		{
			if(KeyNum==10) 
			{
				KeyNum=0;
			}
			if(KeyNum>=0&&KeyNum<=10) //����ȡ����
			{							
				Show_Num+=8;									//��ʾ����8�����ӣ�����ռ8�����أ�
				Sum=Sum*10+KeyNum;								//ͨ���۳�ʵ�ֱ��������ֵ
				OLED_ShowNum(Show_Num, 16,KeyNum, 1,8);
				OLED_Update();
				KeyNum=0;
			}
			if(KeyNum==12&&Show_Num>-8)//ɾ��
			{
				Show_Num-=8;									//��ʾ����8�����ӣ�����ռ8�����أ�
				Sum=(Sum/10)-((Sum%10)/10);								//ͨ���۳�ʵ�ֱ��������ֵ
				OLED_ShowString(Show_Num+8,16," ",8);
				OLED_Update();
				KeyNum=0;
			}
			if(KeyNum==16)//�˳�
			{
				if(JD1==0&&JD2==0&&JD3==0&&JD4==0)
				{
					OLED_Clear();
					return;
				}
				
			}
			if(KeyNum==11)//����ȡ�����ȷ��
			{
				sprintf(TX,"ȡ����1:%lld,ȡ����2:%lld,ȡ����3:%lld,ȡ����4:%lld\r\n",QuJianMa1,QuJianMa2,QuJianMa3,QuJianMa4);
				UART3SendString((u8*)TX,strlen((char*)TX));
				/*�жϵ������ӵ�ȡ����*/////////////////////////////////////////////////////////
				if(Sum!=QuJianMa1&&Sum!=QuJianMa2&&Sum!=QuJianMa3&&Sum!=QuJianMa4&&K==0)
				{
					Usart_SendString(USART1,"QuJianMaCuoWu");
					OLED_ShowChinese(24,32,"��Чȡ����");
					OLED_ShowChinese(24,48,"����������");
					OLED_Update();
					delay_ms(1500);
					OLED_ClearArea(0,32,128,32);
					OLED_ClearArea(0,16,128,16);
					OLED_Update();
					Sum=0; 
					Show_Num=-8;
				}
				if(Sum==QuJianMa1&&GUI1==1)//�������ȡ�������1�Ź�ȡ������һ�Ź��д��
				{
					Usart_SendString(USART1,"GUI1_ON");//������ģ�鷢��ָ������ظ���һ�Ź����Ѵ򿪣������ǹ���
					JD1=90;
					Servo_SetAngle1(JD1);//���ת��90�ȣ����Ŵ�
					GUI1=0;QuJianMa1=0;
					OLED_ClearArea(0,32,128,32);
					OLED_ShowNum(12,32,1,1,8);
					OLED_ShowChinese(20,32,"�Ź����Ѵ�");
					OLED_ShowChinese(16,48,"�����ǹ���Ŷ");
					OLED_Update();
				}
				if(Sum==QuJianMa2&&GUI2==1)
				{
					Usart_SendString(USART1,"GUI2_ON");
					JD2=90;QuJianMa2=0;
					Servo_SetAngle2(JD2);
					GUI2=0;
					OLED_ClearArea(0,32,128,32);
					OLED_ShowNum(12,32,2,1,8);
					OLED_ShowChinese(20,32,"�Ź����Ѵ�");
					OLED_ShowChinese(16,48,"�����ǹ���Ŷ");
					OLED_Update();
				}
				if((Sum==QuJianMa3)&&GUI3==1)
				{
					Usart_SendString(USART1,"GUI3_ON");
					JD3=90;QuJianMa3=0;
					Servo_SetAngle3(JD3);
					GUI3=0;
					OLED_ClearArea(0,32,128,32);
					OLED_ShowNum(12,32,3,1,8);
					OLED_ShowChinese(20,32,"�Ź����Ѵ�");
					OLED_ShowChinese(16,48,"�����ǹ���Ŷ");
					OLED_Update();
					sprintf(TX,"SUM:%lld,111\r\n",Sum);
					UART3SendString((u8*)TX,strlen((char*)TX));
				}
				if(Sum==QuJianMa4&&GUI4==1)
				{
					Usart_SendString(USART1,"GUI4_ON");
					JD4=90;QuJianMa4=0;
					Servo_SetAngle4(JD4);
					GUI4=0;
					OLED_ClearArea(0,32,128,32);
					OLED_ShowNum(12,32,4,1,8);
					OLED_ShowChinese(20,32,"�Ź����Ѵ�");
					OLED_ShowChinese(16,48,"�����ǹ���Ŷ");
				}
				/*�ж����������Ƿ���1�Ź��ֻ�����ͬ���������Ӽ����ϣ�*/
				if(K==123&&GUI2==1&&GUI3==1)
				{
					Usart_SendString(USART1,"GUI23_ON");
					JD2=90;JD3=90;QuJianMa2=0;QuJianMa3=0;
					K=0;
					Servo_SetAngle2(JD2);
					Servo_SetAngle3(JD3);
					GUI2=0;GUI3=0;
					OLED_ClearArea(0,32,128,32);
					OLED_ShowString(10,32,"2,3",8);
					OLED_ShowChinese(34,32,"�Ź��Ѵ�");
					OLED_ShowChinese(16,48,"�����ǹ���Ŷ");
					sprintf(TX,"GUI2:%d,GUI3:%d,\r\n",GUI2,GUI3);
					UART3SendString((u8*)TX,strlen((char*)TX));
				}
				if(K==134&&GUI4==1&&GUI3==1)
				{
					Usart_SendString(USART1,"GUI34_ON");
					JD3=90;JD4=90;QuJianMa4=0;QuJianMa3=0;
					K=0;
					Servo_SetAngle4(JD4);
					Servo_SetAngle3(JD3);
					GUI4=0;GUI3=0;
					OLED_ClearArea(0,32,128,32);
					OLED_ShowString(10,32,"3,4",8);
					OLED_ShowChinese(34,32,"�Ź��Ѵ�");
					OLED_ShowChinese(16,48,"�����ǹ���Ŷ");
					sprintf(TX,"GUI3:%d,GUI4:%d,\r\n",GUI3,GUI4);
					UART3SendString((u8*)TX,strlen((char*)TX));
				}
				if(K==124&&GUI2==1&&GUI4==1)
				{
					Usart_SendString(USART1,"GUI24_ON");
					JD2=90;JD4=90;QuJianMa2=0;QuJianMa4=0;
					K=0;
					Servo_SetAngle4(JD4);
					Servo_SetAngle2(JD2);
					GUI4=0;GUI2=0;
					OLED_ClearArea(0,32,128,32);
					OLED_ShowString(10,32,"2,4",8);
					OLED_ShowChinese(34,32,"�Ź��Ѵ�");
					OLED_ShowChinese(16,48,"�����ǹ���Ŷ");
					sprintf(TX,"GUI2:%d,GUI4:%d,\r\n",GUI2,GUI4);
					UART3SendString((u8*)TX,strlen((char*)TX));
				}
				if(K==1234&&GUI2==1&&GUI4==1&&GUI3==1)
				{
					Usart_SendString(USART1,"GUI234_ON");
					JD2=90;
					JD3=90;
					JD4=90;QuJianMa2=0;QuJianMa3=0;QuJianMa4=0;
					K=0;
					Servo_SetAngle4(JD4);
					Servo_SetAngle2(JD2);
					Servo_SetAngle3(JD3);
					GUI4=0;GUI2=0;GUI3=0;
					OLED_ClearArea(0,32,128,32);
					OLED_ShowString(4,32,"2,3,4",8);
					OLED_ShowChinese(44,32,"�Ź��Ѵ�");
					OLED_ShowChinese(16,48,"�����ǹ���Ŷ");
					sprintf(TX,"GUI2:%d,GUI3:%d,,GUI4:%d\r\n",GUI2,GUI3,GUI4);
					UART3SendString((u8*)TX,strlen((char*)TX));
				}
				/*�ж����������Ƿ���2�Ź��ֻ�����ͬ���������Ӽ����ϣ�*/
				if(K==213&&GUI1==1&&GUI3==1)
				{
					Usart_SendString(USART1,"GUI13_ON");
					JD1=90;
					JD3=90;QuJianMa1=0;QuJianMa3=0;
					K=0;
					Servo_SetAngle1(JD1);
					Servo_SetAngle3(JD3);
					GUI1=0;GUI3=0;
					OLED_ClearArea(0,32,128,32);
					OLED_ShowString(10,32,"1,3",8);
					OLED_ShowChinese(34,32,"�Ź��Ѵ�");
					OLED_ShowChinese(16,48,"�����ǹ���Ŷ");
					sprintf(TX,"GUI1:%d,GUI3:%d\r\n",GUI1,GUI3);
					UART3SendString((u8*)TX,strlen((char*)TX));
				}
				if(K==234&&GUI4==1&&GUI3==1)
				{
					Usart_SendString(USART1,"GUI34_ON");
					JD4=90;
					JD3=90;QuJianMa4=0;QuJianMa3=0;
					K=0;
					Servo_SetAngle4(JD4);
					Servo_SetAngle3(JD3);
					GUI4=0;GUI3=0;
					OLED_ClearArea(0,32,128,32);
					OLED_ShowString(10,32,"3,4",8);
					OLED_ShowChinese(34,32,"�Ź��Ѵ�");
					OLED_ShowChinese(16,48,"�����ǹ���Ŷ");
					sprintf(TX,"GUI3:%d,GUI4:%d\r\n",GUI3,GUI4);
					UART3SendString((u8*)TX,strlen((char*)TX));
				}
				if(K==214&&GUI4==1&&GUI1==1)
				{
					Usart_SendString(USART1,"GUI14_ON");
					JD4=90;
					JD1=90;QuJianMa1=0;QuJianMa4=0;
					K=0;
					Servo_SetAngle4(JD4);
					Servo_SetAngle1(JD1);
					GUI4=0;GUI1=0;
					OLED_ClearArea(0,32,128,32);
					OLED_ShowString(10,32,"1,4",8);
					OLED_ShowChinese(34,32,"�Ź��Ѵ�");
					OLED_ShowChinese(16,48,"�����ǹ���Ŷ");
					sprintf(TX,"GUI1:%d,GUI4:%d\r\n",GUI1,GUI4);
					UART3SendString((u8*)TX,strlen((char*)TX));
				}
				if(K==2134&&GUI1==1&&GUI4==1&&GUI3==1)
				{
					Usart_SendString(USART1,"GUI134_ON");
					JD1=90;
					JD3=90;
					JD4=90;QuJianMa1=0;QuJianMa3=0;QuJianMa4=0;
					K=0;
					Servo_SetAngle4(JD4);
					Servo_SetAngle1(JD1);
					Servo_SetAngle3(JD3);
					GUI4=0;GUI1=0;GUI3=0;
					OLED_ClearArea(0,32,128,32);
					OLED_ShowString(4,32,"1,3,4",8);
					OLED_ShowChinese(44,32,"�Ź��Ѵ�");
					OLED_ShowChinese(16,48,"�����ǹ���Ŷ");
					sprintf(TX,"GUI1:%d,GUI3:%d,,GUI4:%d\r\n",GUI1,GUI3,GUI4);
					UART3SendString((u8*)TX,strlen((char*)TX));
				}
				/*�ж����������Ƿ���3�Ź��ֻ�����ͬ���������Ӽ����ϣ�*/
				if(K==312&&GUI1==1&&GUI2==1)
				{
					Usart_SendString(USART1,"GUI12_ON");
					JD1=90;
					JD2=90;QuJianMa1=0;QuJianMa2=0;
					K=0;
					Servo_SetAngle1(JD1);
					Servo_SetAngle2(JD2);
					GUI1=0;GUI2=0;
					OLED_ClearArea(0,32,128,32);
					OLED_ShowString(10,32,"1,2",8);
					OLED_ShowChinese(34,32,"�Ź��Ѵ�");
					OLED_ShowChinese(16,48,"�����ǹ���Ŷ");
					sprintf(TX,"GUI1:%d,GUI2:%d\r\n",GUI1,GUI2);
					UART3SendString((u8*)TX,strlen((char*)TX));
				}
				if(K==324&&GUI4==1&&GUI2==1)
				{
					Usart_SendString(USART1,"GUI24_ON");
					JD4=90;
					JD2=90;QuJianMa4=0;QuJianMa2=0;
					K=0;
					Servo_SetAngle4(JD4);
					Servo_SetAngle2(JD2);
					GUI4=0;GUI2=0;
					OLED_ClearArea(0,32,128,32);
					OLED_ShowString(10,32,"2,4",8);
					OLED_ShowChinese(34,32,"�Ź��Ѵ�");
					OLED_ShowChinese(16,48,"�����ǹ���Ŷ");
					sprintf(TX,"GUI2:%d,GUI4:%d\r\n",GUI2,GUI4);
					UART3SendString((u8*)TX,strlen((char*)TX));
				}
				if(K==314&&GUI4==1&&GUI1==1)
				{
					Usart_SendString(USART1,"GUI14_ON");
					JD1=90;
					JD4=90;QuJianMa1=0;QuJianMa4=0;
					K=0;
					Servo_SetAngle4(JD4);
					Servo_SetAngle1(JD1);
					GUI4=0;GUI1=0;
					OLED_ClearArea(0,32,128,32);
					OLED_ShowString(10,32,"1,4",8);
					OLED_ShowChinese(34,32,"�Ź��Ѵ�");
					OLED_ShowChinese(16,48,"�����ǹ���Ŷ");
					sprintf(TX,"GUI1:%d,GUI4:%d\r\n",GUI1,GUI4);
					UART3SendString((u8*)TX,strlen((char*)TX));
				}
				if(K==3124&&GUI4==1&&GUI1==1&&GUI2==1)
				{
					Usart_SendString(USART1,"GUI124_ON");
					JD1=90;
					JD2=90;
					JD4=90;QuJianMa1=0;QuJianMa2=0;QuJianMa4=0;
					K=0;
					Servo_SetAngle4(JD4);
					Servo_SetAngle2(JD2);
					Servo_SetAngle1(JD1);
					GUI4=0;GUI1=0;GUI2=0;
					OLED_ClearArea(0,32,128,32);
					OLED_ShowString(4,32,"1,2,4",8);
					OLED_ShowChinese(44,32,"�Ź��Ѵ�");
					OLED_ShowChinese(16,48,"�����ǹ���Ŷ");
					sprintf(TX,"GUI1:%d,GUI2:%d,GUI4:%d\r\n",GUI1,GUI2,GUI4);
					UART3SendString((u8*)TX,strlen((char*)TX));
				}
				/*�ж����������Ƿ���4�Ź��ֻ�����ͬ���������Ӽ����ϣ�*/
				if(K==412&&GUI2==1&&GUI1==1)
				{
					Usart_SendString(USART1,"GUI12_ON");
					JD1=90;
					JD2=90;QuJianMa1=0;QuJianMa2=0;
					K=0;
					Servo_SetAngle2(JD2);
					Servo_SetAngle1(JD1);
					GUI2=0;GUI1=0;
					OLED_ClearArea(0,32,128,32);
				OLED_ShowString(10,32,"1,2",8);
					OLED_ShowChinese(34,32,"�Ź��Ѵ�");
					OLED_ShowChinese(16,48,"�����ǹ���Ŷ");
					sprintf(TX,"GUI1:%d,GUI2:%d\r\n",GUI1,GUI2);
					UART3SendString((u8*)TX,strlen((char*)TX));
				}
				if(K==413&&GUI3==1&&GUI1==1)
				{
					Usart_SendString(USART1,"GUI13_ON");
					JD1=90;
					JD3=90;QuJianMa1=0;QuJianMa3=0;
					K=0;
					Servo_SetAngle3(JD3);
					Servo_SetAngle1(JD1);
					GUI3=0;GUI1=0;
					OLED_ClearArea(0,32,128,32);
					OLED_ShowString(10,32,"1,3",8);
					OLED_ShowChinese(34,32,"�Ź��Ѵ�");
					OLED_ShowChinese(16,48,"�����ǹ���Ŷ");
					sprintf(TX,"GUI1:%d,GUI3:%d\r\n",GUI1,GUI3);
					UART3SendString((u8*)TX,strlen((char*)TX));
				}
				if(K==423&&GUI3==1&&GUI2==1)
				{
					Usart_SendString(USART1,"GUI23_ON");
					JD2=90;
					JD3=90;QuJianMa3=0;QuJianMa2=0;
					K=0;
					Servo_SetAngle3(JD3);
					Servo_SetAngle2(JD2);
					GUI3=0;GUI2=0;
					OLED_ClearArea(0,32,128,32);
					OLED_ShowString(10,32,"2,3",8);
					OLED_ShowChinese(34,32,"�Ź��Ѵ�");
					OLED_ShowChinese(16,48,"�����ǹ���Ŷ");
					sprintf(TX,"GUI2:%d,GUI3:%d\r\n",GUI2,GUI3);
					UART3SendString((u8*)TX,strlen((char*)TX));
				}
				if(K==4123&&GUI3==1&&GUI2==1&&GUI1==1)
				{
					Usart_SendString(USART1,"GUI123_ON");
					JD1=90;
					JD2=90;
					JD3=90;QuJianMa1=0;QuJianMa2=0;QuJianMa3=0;
					K=0;
					Servo_SetAngle3(JD3);
					Servo_SetAngle2(JD2);
					Servo_SetAngle1(JD1);
					GUI3=0;GUI2=0;GUI1=0;
					OLED_ClearArea(0,32,128,32);
					OLED_ShowString(4,32,"1,2,3",8);
					OLED_ShowChinese(44,32,"�Ź��Ѵ�");
					OLED_ShowChinese(16,48,"�����ǹ���Ŷ");
					sprintf(TX,"GUI1:%d,GUI2:%d,GUI3:%d\r\n",GUI1,GUI2,GUI3);
					UART3SendString((u8*)TX,strlen((char*)TX));
				}
				OLED_Update();
				KeyNum=!11;
			}
			if(KeyNum==15)//�ع��ţ����ж��Ƿ�����ͬ���ֻ���
			{
				/*�رմ򿪵���������*//////////////////////////////////////////////////////////////////////////
				if(JD2==90&&JD3==90&&JD4==90)
				{
					Usart_SendString(USART1,"GUI_OFF");
					JD2=0;JD3=0;JD4=0;
					Servo_SetAngle2(JD2);
					Servo_SetAngle3(JD3);
					Servo_SetAngle4(JD4);
					QuJianMa2PhoneNum=0;QuJianMa3PhoneNum=0;QuJianMa4PhoneNum=0;
					OLED_ClearArea(0,32,128,32);
					OLED_ShowChinese(24,32,"�����ѹر�");
					OLED_Update();
					delay_ms(1500);
					OLED_ClearArea(0,32,128,32);
					OLED_ClearArea(0,16,128,16);
					OLED_Update();
					Sum=0; 
					Show_Num=-8;
//					QuJianMa2=0;QuJianMa3=0;QuJianMa4=0;
				}
				if(JD1==90&&JD3==90&&JD4==90)
				{
					Usart_SendString(USART1,"GUI_OFF");
					JD1=0;JD3=0;JD4=0;
					Servo_SetAngle1(JD1);
					Servo_SetAngle3(JD3);
					Servo_SetAngle4(JD4);
					QuJianMa1PhoneNum=0;QuJianMa3PhoneNum=0;QuJianMa4PhoneNum=0;
					OLED_ClearArea(0,32,128,32);
					OLED_ShowChinese(24,32,"�����ѹر�");
					OLED_Update();
					delay_ms(1500);
					OLED_ClearArea(0,32,128,32);
					OLED_ClearArea(0,16,128,16);
					OLED_Update();
					Sum=0; 
					Show_Num=-8;
//					QuJianMa1=0;QuJianMa3=0;QuJianMa4=0;
				}
				if(JD1==90&&JD2==90&&JD3==90)
				{
					Usart_SendString(USART1,"GUI_OFF");
					JD1=0;JD2=0;JD3=0;
					Servo_SetAngle1(JD1);
					Servo_SetAngle2(JD2);
					Servo_SetAngle3(JD3);
					QuJianMa1PhoneNum=0;QuJianMa2PhoneNum=0;QuJianMa3PhoneNum=0;
					OLED_ClearArea(0,32,128,32);
					OLED_ShowChinese(24,32,"�����ѹر�");
					OLED_Update();
					delay_ms(1500);
					OLED_ClearArea(0,32,128,32);
					OLED_ClearArea(0,16,128,16);
					OLED_Update();
					Sum=0; 
					Show_Num=-8;
//					QuJianMa1=0;QuJianMa2=0;QuJianMa3=0;
				}
				if(JD1==90&&JD2==90&&JD4==90)
				{
					Usart_SendString(USART1,"GUI_OFF");
					JD1=0;JD2=0;JD4=0;
					Servo_SetAngle1(JD1);
					Servo_SetAngle2(JD2);
					Servo_SetAngle4(JD4);
					QuJianMa1PhoneNum=0;QuJianMa2PhoneNum=0;QuJianMa4PhoneNum=0;
					OLED_ClearArea(0,32,128,32);
					OLED_ShowChinese(24,32,"�����ѹر�");
					OLED_Update();
					delay_ms(1500);
					OLED_ClearArea(0,32,128,32);
					OLED_ClearArea(0,16,128,16);
					OLED_Update();
					Sum=0; 
					Show_Num=-8;
//					QuJianMa1=0;QuJianMa2=0;QuJianMa4=0;
				}
				/*�رմ򿪵���������*//////////////////////////////////////////////////////////////////////////
				if(JD2==90&&JD3==90)
				{
					Usart_SendString(USART1,"GUI_OFF");
					JD2=0;JD3=0;
					Servo_SetAngle2(JD2);
					Servo_SetAngle3(JD3);
					QuJianMa2PhoneNum=0;QuJianMa3PhoneNum=0;
					OLED_ClearArea(0,32,128,32);
					OLED_ShowChinese(24,32,"�����ѹر�");
					OLED_Update();
					delay_ms(1500);
					OLED_ClearArea(0,32,128,32);
					OLED_ClearArea(0,16,128,16);
					OLED_Update();
					Sum=0; 
					Show_Num=-8;
//					QuJianMa2=0;QuJianMa3=0;
				}
				if(JD3==90&&JD4==90)
				{
					Usart_SendString(USART1,"GUI_OFF");
					JD3=0;JD4=0;
					Servo_SetAngle3(JD3);
					Servo_SetAngle4(JD4);
					QuJianMa3PhoneNum=0;QuJianMa4PhoneNum=0;
					OLED_ClearArea(0,32,128,32);
					OLED_ShowChinese(24,32,"�����ѹر�");
					OLED_Update();
					delay_ms(1500);
					OLED_ClearArea(0,32,128,32);
					OLED_ClearArea(0,16,128,16);
					OLED_Update();
					Sum=0; 
					Show_Num=-8;
//					QuJianMa3=0;QuJianMa4=0;
				}
				if(JD2==90&&JD4==90)
				{
					Usart_SendString(USART1,"GUI_OFF");
					JD2=0;JD4=0;
					Servo_SetAngle2(JD2);
					Servo_SetAngle4(JD4);
					QuJianMa2PhoneNum=0;QuJianMa4PhoneNum=0;
					OLED_ClearArea(0,32,128,32);
					OLED_ShowChinese(24,32,"�����ѹر�");
					OLED_Update();
					delay_ms(1500);
					OLED_ClearArea(0,32,128,32);
					OLED_ClearArea(0,16,128,16);
					OLED_Update();
					Sum=0; 
					Show_Num=-8;
//					QuJianMa2=0;QuJianMa4=0;
				}
				if(JD1==90&&JD3==90)
				{
					Usart_SendString(USART1,"GUI_OFF");
					JD1=0;JD3=0;
					Servo_SetAngle1(JD1);
					Servo_SetAngle3(JD3);
					QuJianMa1PhoneNum=0;QuJianMa3PhoneNum=0;
					OLED_ClearArea(0,32,128,32);
					OLED_ShowChinese(24,32,"�����ѹر�");
					OLED_Update();
					delay_ms(1500);
					OLED_ClearArea(0,32,128,32);
					OLED_ClearArea(0,16,128,16);
					OLED_Update();
					Sum=0; 
					Show_Num=-8;
//					QuJianMa1=0;QuJianMa3=0;
				}
				if(JD1==90&&JD4==90)
				{
					Usart_SendString(USART1,"GUI_OFF");
					JD1=0;JD4=0;
					Servo_SetAngle1(JD1);
					Servo_SetAngle4(JD4);
					QuJianMa1PhoneNum=0;QuJianMa4PhoneNum=0;
					OLED_ClearArea(0,32,128,32);
					OLED_ShowChinese(24,32,"�����ѹر�");
					OLED_Update();
					delay_ms(1500);
					OLED_ClearArea(0,32,128,32);
					OLED_ClearArea(0,16,128,16);
					OLED_Update();
					Sum=0; 
					Show_Num=-8;
//					QuJianMa1=0;QuJianMa4=0;
				}
				if(JD1==90&&JD2==90)
				{
					Usart_SendString(USART1,"GUI_OFF");
					JD1=0;JD2=0;
					Servo_SetAngle1(JD1);
					Servo_SetAngle2(JD2);
					QuJianMa1PhoneNum=0;QuJianMa2PhoneNum=0;
					OLED_ClearArea(0,32,128,32);
					OLED_ShowChinese(24,32,"�����ѹر�");
					OLED_Update();
					delay_ms(1500);
					OLED_ClearArea(0,32,128,32);
					OLED_ClearArea(0,16,128,16);
					OLED_Update();
					Sum=0; 
					Show_Num=-8;
//					QuJianMa1=0;QuJianMa2=0;
				}
				if(JD1==90&&JD2==90)
				{
					Usart_SendString(USART1,"GUI_OFF");
					JD1=0;JD2=0;
					Servo_SetAngle1(JD1);
					Servo_SetAngle2(JD2);
					QuJianMa1PhoneNum=0;QuJianMa2PhoneNum=0;
					OLED_ClearArea(0,32,128,32);
					OLED_ShowChinese(24,32,"�����ѹر�");
					OLED_Update();
					delay_ms(1500);
					OLED_ClearArea(0,32,128,32);
					OLED_ClearArea(0,16,128,16);
					OLED_Update();
					Sum=0; 
					Show_Num=-8;
//					QuJianMa1=0;QuJianMa2=0;
				}	
				/*�ر�1�Ź��ţ��ж����������ֻ����Ƿ���1�Ź�����ͬ*//////////////////////////////////////////////////////////////////////////				
				if(JD1!=0&&JD1==90&&JD2==0&&JD3==0&&JD4==0)
				{
					JD1=0;JD2=0;JD3=0;JD4=0;
					Servo_SetAngle1(JD1);
					Servo_SetAngle2(JD2);
					Servo_SetAngle3(JD3);
					Servo_SetAngle4(JD4);
//					QuJianMa1=0;
					/*2��3��4�Ź��Ƿ��1�Ź���ֻ�����ͬ*/
					if(QuJianMa1PhoneNum==QuJianMa2PhoneNum&&QuJianMa3PhoneNum==QuJianMa1PhoneNum&&QuJianMa4PhoneNum==QuJianMa1PhoneNum&&QuJianMa1PhoneNum!=0)
					{
						Usart_SendString(USART1,"HaiYouJian");
						OLED_ClearArea(0,32,128,32);
						OLED_ShowChinese(16,32,"������δȡ��");
						OLED_ShowChinese(32,48,"�Ƿ�ȡ��");
						OLED_Update();
						K=1234;
						sprintf(TX,"K:%d,%lld,%lld,%lld\r\n",K,QuJianMa1PhoneNum,QuJianMa2PhoneNum,QuJianMa3PhoneNum);
						UART3SendString((u8*)TX,strlen((char*)TX));
					}
					/*2��3�Ź��Ƿ��1�Ź���ֻ�����ͬ*/
					else if(QuJianMa1PhoneNum==QuJianMa2PhoneNum&&QuJianMa3PhoneNum==QuJianMa1PhoneNum&&QuJianMa1PhoneNum!=0)
					{
						Usart_SendString(USART1,"HaiYouJian");
						OLED_ClearArea(0,32,128,32);
						OLED_ShowChinese(16,32,"������δȡ��");
						OLED_ShowChinese(32,48,"�Ƿ�ȡ��");
						OLED_Update();
						K=123;
						sprintf(TX,"K:%d,%lld,%lld,%lld\r\n",K,QuJianMa1PhoneNum,QuJianMa2PhoneNum,QuJianMa3PhoneNum);
						UART3SendString((u8*)TX,strlen((char*)TX));
					}
					/*3��4�Ź��Ƿ��1�Ź���ֻ�����ͬ*/
					else if(QuJianMa1PhoneNum==QuJianMa3PhoneNum&&QuJianMa4PhoneNum==QuJianMa1PhoneNum&&QuJianMa1PhoneNum!=0)
					{
						Usart_SendString(USART1,"HaiYouJian");
						OLED_ClearArea(0,32,128,32);
						OLED_ShowChinese(16,32,"������δȡ��");
						OLED_ShowChinese(32,48,"�Ƿ�ȡ��");
						OLED_Update();
						K=134;
						sprintf(TX,"K:%d\r\n",K);
						UART3SendString((u8*)TX,strlen((char*)TX));
					}
					/*2��4�Ź��Ƿ��1�Ź���ֻ�����ͬ*/
					else if(QuJianMa1PhoneNum==QuJianMa2PhoneNum&&QuJianMa4PhoneNum==QuJianMa1PhoneNum&&QuJianMa1PhoneNum!=0)
					{
						Usart_SendString(USART1,"HaiYouJian");
//						QuJianMa1PhoneNum=0;QuJianMa2PhoneNum=0;QuJianMa4PhoneNum=0;
						OLED_ClearArea(0,32,128,32);
						OLED_ShowChinese(16,32,"������δȡ��");
						OLED_ShowChinese(32,48,"�Ƿ�ȡ��");
						OLED_Update();
						K=124;
						sprintf(TX,"K:%d,%lld,%lld,%lld\r\n",K,QuJianMa1PhoneNum,QuJianMa2PhoneNum,QuJianMa3PhoneNum);
						UART3SendString((u8*)TX,strlen((char*)TX));
					}
					/*2�Ź��Ƿ��1�Ź���ֻ�����ͬ*/
					else if(QuJianMa2PhoneNum==QuJianMa1PhoneNum&&QuJianMa3PhoneNum!=QuJianMa1PhoneNum&&QuJianMa4PhoneNum!=QuJianMa1PhoneNum&&QuJianMa1PhoneNum!=0)
					{
						Usart_SendString(USART1,"HaiYouJian");
//						QuJianMa1PhoneNum=0;QuJianMa2PhoneNum=0;
						OLED_ClearArea(0,32,128,32);
						OLED_ShowChinese(16,32,"������δȡ��");
						OLED_ShowChinese(32,48,"�Ƿ�ȡ��");
						OLED_Update();
						Sum=QuJianMa2;
						sprintf(TX,"ȡ����2:%lld\r\n",QuJianMa2);
						UART3SendString((u8*)TX,strlen((char*)TX));
						
					}
					/*3�Ź��Ƿ��1�Ź���ֻ�����ͬ*/
					else if(QuJianMa3PhoneNum==QuJianMa1PhoneNum&&QuJianMa4PhoneNum!=QuJianMa1PhoneNum&&QuJianMa2PhoneNum!=QuJianMa1PhoneNum&&QuJianMa1PhoneNum!=0)
					{
						Usart_SendString(USART1,"HaiYouJian");
//						QuJianMa1PhoneNum=0;QuJianMa3PhoneNum=0;
						OLED_ClearArea(0,32,128,32);
						OLED_ShowChinese(16,32,"������δȡ��");
						OLED_ShowChinese(32,48,"�Ƿ�ȡ��");
						OLED_Update();
						Sum=QuJianMa3;
						sprintf(TX,"ȡ����3:%lld\r\n",QuJianMa3);
						UART3SendString((u8*)TX,strlen((char*)TX));
					}
					/*4�Ź��Ƿ��1�Ź���ֻ�����ͬ*/
					else if(QuJianMa4PhoneNum==QuJianMa1PhoneNum&&QuJianMa2PhoneNum!=QuJianMa1PhoneNum&&QuJianMa3PhoneNum!=QuJianMa1PhoneNum&&QuJianMa1PhoneNum!=0)
					{
						Usart_SendString(USART1,"HaiYouJian");
//						QuJianMa1PhoneNum=0;QuJianMa4PhoneNum=0;
						OLED_ClearArea(0,32,128,32);
						OLED_ShowChinese(16,32,"������δȡ��");
						OLED_ShowChinese(32,48,"�Ƿ�ȡ��");
						OLED_Update();
						Sum=QuJianMa4;
						sprintf(TX,"ȡ����4:%lld\r\n",QuJianMa4);
						UART3SendString((u8*)TX,strlen((char*)TX));
					}
					/*û�й��ӵ��ֻ�����1�Ź���ֻ�����ͬ*/
					else if(QuJianMa4PhoneNum!=QuJianMa1PhoneNum&&QuJianMa2PhoneNum!=QuJianMa1PhoneNum&&QuJianMa3PhoneNum!=QuJianMa1PhoneNum&&QuJianMa1PhoneNum!=0)
					{
						Usart_SendString(USART1,"GUI_OFF");
						OLED_ClearArea(0,32,128,32);
						OLED_ShowChinese(24,32,"�����ѹر�");
						OLED_Update();
						delay_ms(1500);
						OLED_ClearArea(0,32,128,32);
						OLED_ClearArea(0,16,128,16);
						OLED_Update();
						Sum=0; 
						Show_Num=-8;
					}
					sprintf(TX,"�ֻ���1:%lld �ֻ���2:%lld\r\n",QuJianMa1PhoneNum,QuJianMa2PhoneNum);
					UART3SendString((u8*)TX,strlen((char*)TX));
					QuJianMa1PhoneNum=0;
				}
				/*�ر�2�Ź��ţ��ж����������ֻ����Ƿ���2�Ź�����ͬ*//////////////////////////////////////////////////////////////////////////								
				if(JD2!=0&&JD2==90&&JD1==0&&JD3==0&&JD4==0)////////////////////
				{
					JD1=0;JD2=0;JD3=0;JD4=0;
					Servo_SetAngle1(JD1);
					Servo_SetAngle2(JD2);
					Servo_SetAngle3(JD3);
					Servo_SetAngle4(JD4);
//					QuJianMa2=0;
					if(QuJianMa1PhoneNum==QuJianMa2PhoneNum&&QuJianMa3PhoneNum==QuJianMa2PhoneNum&&QuJianMa4PhoneNum==QuJianMa2PhoneNum&&QuJianMa2PhoneNum!=0)
					{
						Usart_SendString(USART1,"HaiYouJian");
//						QuJianMa1PhoneNum=0;QuJianMa2PhoneNum=0;QuJianMa3PhoneNum=0;QuJianMa4PhoneNum=0;
						OLED_ClearArea(0,32,128,32);
						OLED_ShowChinese(16,32,"������δȡ��");
						OLED_ShowChinese(32,48,"�Ƿ�ȡ��");
						OLED_Update();
						K=2134;
						sprintf(TX,"K:%d,%lld,%lld,%lld\r\n",K,QuJianMa1PhoneNum,QuJianMa3PhoneNum,QuJianMa4PhoneNum);
						UART3SendString((u8*)TX,strlen((char*)TX));
					}
					else if(QuJianMa1PhoneNum==QuJianMa2PhoneNum&&QuJianMa3PhoneNum==QuJianMa2PhoneNum&&QuJianMa2PhoneNum!=0)
					{
						Usart_SendString(USART1,"HaiYouJian");
//						QuJianMa1PhoneNum=0;QuJianMa2PhoneNum=0;QuJianMa3PhoneNum=0;
						OLED_ClearArea(0,32,128,32);
						OLED_ShowChinese(16,32,"������δȡ��");
						OLED_ShowChinese(32,48,"�Ƿ�ȡ��");
						OLED_Update();
						K=213;
						sprintf(TX,"K:%d,%lld,%lld,%lld\r\n",K,QuJianMa1PhoneNum,QuJianMa2PhoneNum,QuJianMa3PhoneNum);
						UART3SendString((u8*)TX,strlen((char*)TX));
					}
					else if(QuJianMa2PhoneNum==QuJianMa3PhoneNum&&QuJianMa4PhoneNum==QuJianMa2PhoneNum&&QuJianMa2PhoneNum!=0)
					{
						Usart_SendString(USART1,"HaiYouJian");
//						QuJianMa2PhoneNum=0;QuJianMa4PhoneNum=0;QuJianMa3PhoneNum=0;
						OLED_ClearArea(0,32,128,32);
						OLED_ShowChinese(16,32,"������δȡ��");
						OLED_ShowChinese(32,48,"�Ƿ�ȡ��");
						OLED_Update();
						K=234;
						sprintf(TX,"K:%d\r\n",K);
						UART3SendString((u8*)TX,strlen((char*)TX));
					}
					else if(QuJianMa1PhoneNum==QuJianMa2PhoneNum&&QuJianMa4PhoneNum==QuJianMa2PhoneNum&&QuJianMa2PhoneNum!=0)
					{
						Usart_SendString(USART1,"HaiYouJian");
//						QuJianMa1PhoneNum=0;QuJianMa2PhoneNum=0;QuJianMa4PhoneNum=0;
						OLED_ClearArea(0,32,128,32);
						OLED_ShowChinese(16,32,"������δȡ��");
						OLED_ShowChinese(32,48,"�Ƿ�ȡ��");
						OLED_Update();
						K=214;
						sprintf(TX,"K:%d,%lld,%lld,%lld\r\n",K,QuJianMa1PhoneNum,QuJianMa2PhoneNum,QuJianMa3PhoneNum);
						UART3SendString((u8*)TX,strlen((char*)TX));
					}
					else if(QuJianMa2PhoneNum==QuJianMa1PhoneNum&&QuJianMa3PhoneNum!=QuJianMa2PhoneNum&&QuJianMa4PhoneNum!=QuJianMa2PhoneNum&&QuJianMa2PhoneNum!=0)
					{
						Usart_SendString(USART1,"HaiYouJian");
//						QuJianMa1PhoneNum=0;QuJianMa2PhoneNum=0;
						OLED_ClearArea(0,32,128,32);
						OLED_ShowChinese(16,32,"������δȡ��");
						OLED_ShowChinese(32,48,"�Ƿ�ȡ��");
						OLED_Update();
						Sum=QuJianMa1;
						sprintf(TX,"ȡ����2:%lld\r\n",QuJianMa2);
						UART3SendString((u8*)TX,strlen((char*)TX));
						
					}
					else if(QuJianMa3PhoneNum==QuJianMa2PhoneNum&&QuJianMa4PhoneNum!=QuJianMa2PhoneNum&&QuJianMa2PhoneNum!=QuJianMa1PhoneNum&&QuJianMa2PhoneNum!=0)
					{
						Usart_SendString(USART1,"HaiYouJian");
//						QuJianMa2PhoneNum=0;QuJianMa3PhoneNum=0;
						OLED_ClearArea(0,32,128,32);
						OLED_ShowChinese(16,32,"������δȡ��");
						OLED_ShowChinese(32,48,"�Ƿ�ȡ��");
						OLED_Update();
						Sum=QuJianMa3;
						sprintf(TX,"ȡ����3:%lld\r\n",QuJianMa3);
						UART3SendString((u8*)TX,strlen((char*)TX));
					}
					else if(QuJianMa4PhoneNum==QuJianMa2PhoneNum&&QuJianMa2PhoneNum!=QuJianMa1PhoneNum&&QuJianMa3PhoneNum!=QuJianMa2PhoneNum&&QuJianMa2PhoneNum!=0)
					{
						Usart_SendString(USART1,"HaiYouJian");
//						QuJianMa2PhoneNum=0;QuJianMa4PhoneNum=0;
						OLED_ClearArea(0,32,128,32);
						OLED_ShowChinese(16,32,"������δȡ��");
						OLED_ShowChinese(32,48,"�Ƿ�ȡ��");
						OLED_Update();
						Sum=QuJianMa4;
						sprintf(TX,"ȡ����4:%lld\r\n",QuJianMa4);
						UART3SendString((u8*)TX,strlen((char*)TX));
					}
					else if(QuJianMa4PhoneNum!=QuJianMa2PhoneNum&&QuJianMa3PhoneNum!=QuJianMa2PhoneNum&&QuJianMa1PhoneNum!=QuJianMa2PhoneNum&&QuJianMa2PhoneNum!=0)
					{
						Usart_SendString(USART1,"GUI_OFF");
						OLED_ClearArea(0,32,128,32);
						OLED_ShowChinese(24,32,"�����ѹر�");
						OLED_Update();
						delay_ms(1500);
						OLED_ClearArea(0,32,128,32);
						OLED_ClearArea(0,16,128,16);
						OLED_Update();
						Sum=0; 
						Show_Num=-8;
					}
					
					sprintf(TX,"�ֻ���1:%lld �ֻ���2:%lld\r\n",QuJianMa1PhoneNum,QuJianMa2PhoneNum);
					UART3SendString((u8*)TX,strlen((char*)TX));
					QuJianMa2PhoneNum=0;
				}
				/*�ر�3�Ź��ţ��ж����������ֻ����Ƿ���3�Ź�����ͬ*//////////////////////////////////////////////////////////////////////////												
				if(JD3!=0&&JD3==90&&JD2==0&&JD1==0&&JD4==0)/////////////////////
				{
					JD1=0;JD2=0;JD3=0;JD4=0;
					Servo_SetAngle1(JD1);
					Servo_SetAngle2(JD2);
					Servo_SetAngle3(JD3);
					Servo_SetAngle4(JD4);
//					QuJianMa3=0;
					if(QuJianMa1PhoneNum==QuJianMa3PhoneNum&&QuJianMa3PhoneNum==QuJianMa2PhoneNum&&QuJianMa4PhoneNum==QuJianMa3PhoneNum&&QuJianMa3PhoneNum!=0)
					{
						Usart_SendString(USART1,"HaiYouJian");
//						QuJianMa1PhoneNum=0;QuJianMa2PhoneNum=0;QuJianMa3PhoneNum=0;QuJianMa4PhoneNum=0;
						OLED_ClearArea(0,32,128,32);
						OLED_ShowChinese(16,32,"������δȡ��");
						OLED_ShowChinese(32,48,"�Ƿ�ȡ��");
						OLED_Update();
						K=3124;
						sprintf(TX,"K:%d,%lld,%lld,%lld\r\n",K,QuJianMa1PhoneNum,QuJianMa2PhoneNum,QuJianMa4PhoneNum);
						UART3SendString((u8*)TX,strlen((char*)TX));
					}
					else if(QuJianMa1PhoneNum==QuJianMa3PhoneNum&&QuJianMa3PhoneNum==QuJianMa2PhoneNum&&QuJianMa3PhoneNum!=0)
					{
						Usart_SendString(USART1,"HaiYouJian");
//						QuJianMa1PhoneNum=0;QuJianMa2PhoneNum=0;QuJianMa3PhoneNum=0;
						OLED_ClearArea(0,32,128,32);
						OLED_ShowChinese(16,32,"������δȡ��");
						OLED_ShowChinese(32,48,"�Ƿ�ȡ��");
						OLED_Update();
						K=312;
						sprintf(TX,"K:%d,%lld,%lld,%lld\r\n",K,QuJianMa1PhoneNum,QuJianMa2PhoneNum,QuJianMa3PhoneNum);
						UART3SendString((u8*)TX,strlen((char*)TX));
					}
					else if(QuJianMa2PhoneNum==QuJianMa3PhoneNum&&QuJianMa4PhoneNum==QuJianMa3PhoneNum&&QuJianMa3PhoneNum!=0)
					{
						Usart_SendString(USART1,"HaiYouJian");
//						QuJianMa2PhoneNum=0;QuJianMa4PhoneNum=0;QuJianMa3PhoneNum=0;
						OLED_ClearArea(0,32,128,32);
						OLED_ShowChinese(16,32,"������δȡ��");
						OLED_ShowChinese(32,48,"�Ƿ�ȡ��");
						OLED_Update();
						K=324;
						sprintf(TX,"K:%d\r\n",K);
						UART3SendString((u8*)TX,strlen((char*)TX));
					}
					else if(QuJianMa1PhoneNum==QuJianMa3PhoneNum&&QuJianMa4PhoneNum==QuJianMa3PhoneNum&&QuJianMa3PhoneNum!=0)
					{
						Usart_SendString(USART1,"HaiYouJian");
//						QuJianMa1PhoneNum=0;QuJianMa2PhoneNum=0;QuJianMa4PhoneNum=0;
						OLED_ClearArea(0,32,128,32);
						OLED_ShowChinese(16,32,"������δȡ��");
						OLED_ShowChinese(32,48,"�Ƿ�ȡ��");
						OLED_Update();
						K=314;
						sprintf(TX,"K:%d,%lld,%lld,%lld\r\n",K,QuJianMa1PhoneNum,QuJianMa2PhoneNum,QuJianMa3PhoneNum);
						UART3SendString((u8*)TX,strlen((char*)TX));
					}
					else if(QuJianMa3PhoneNum==QuJianMa1PhoneNum&&QuJianMa3PhoneNum!=QuJianMa2PhoneNum&&QuJianMa4PhoneNum!=QuJianMa3PhoneNum&&QuJianMa3PhoneNum!=0)
					{
						Usart_SendString(USART1,"HaiYouJian");
//						QuJianMa1PhoneNum=0;QuJianMa2PhoneNum=0;
						OLED_ClearArea(0,32,128,32);
						OLED_ShowChinese(16,32,"������δȡ��");
						OLED_ShowChinese(32,48,"�Ƿ�ȡ��");
						OLED_Update();
						Sum=QuJianMa1;
						sprintf(TX,"ȡ����1:%lld\r\n",QuJianMa1);
						UART3SendString((u8*)TX,strlen((char*)TX));
						
					}
					else if(QuJianMa3PhoneNum==QuJianMa2PhoneNum&&QuJianMa4PhoneNum!=QuJianMa3PhoneNum&&QuJianMa3PhoneNum!=QuJianMa1PhoneNum&&QuJianMa3PhoneNum!=0)
					{
						Usart_SendString(USART1,"HaiYouJian");
//						QuJianMa2PhoneNum=0;QuJianMa3PhoneNum=0;
						OLED_ClearArea(0,32,128,32);
						OLED_ShowChinese(16,32,"������δȡ��");
						OLED_ShowChinese(32,48,"�Ƿ�ȡ��");
						OLED_Update();
						Sum=QuJianMa2;
						sprintf(TX,"ȡ����2:%lld\r\n",QuJianMa2);
						UART3SendString((u8*)TX,strlen((char*)TX));
					}
					else if(QuJianMa4PhoneNum==QuJianMa3PhoneNum&&QuJianMa3PhoneNum!=QuJianMa1PhoneNum&&QuJianMa3PhoneNum!=QuJianMa2PhoneNum&&QuJianMa3PhoneNum!=0)
					{
						Usart_SendString(USART1,"HaiYouJian");
//						QuJianMa2PhoneNum=0;QuJianMa4PhoneNum=0;
						OLED_ClearArea(0,32,128,32);
						OLED_ShowChinese(16,32,"������δȡ��");
						OLED_ShowChinese(32,48,"�Ƿ�ȡ��");
						OLED_Update();
						Sum=QuJianMa4;
						sprintf(TX,"ȡ����4:%lld\r\n",QuJianMa4);
						UART3SendString((u8*)TX,strlen((char*)TX));
					}
					else if(QuJianMa4PhoneNum!=QuJianMa3PhoneNum&&QuJianMa2PhoneNum!=QuJianMa3PhoneNum&&QuJianMa1PhoneNum!=QuJianMa3PhoneNum&&QuJianMa3PhoneNum!=0)
					{
						Usart_SendString(USART1,"GUI_OFF");
						OLED_ClearArea(0,32,128,32);
						OLED_ShowChinese(24,32,"�����ѹر�");
						OLED_Update();
						delay_ms(1500);
						OLED_ClearArea(0,32,128,32);
						OLED_ClearArea(0,16,128,16);
						OLED_Update();
						Sum=0; 
						Show_Num=-8;
					}
//					sprintf(TX,"�ֻ���1:%lld �ֻ���2:%lld\r\n",QuJianMa1PhoneNum,QuJianMa2PhoneNum);
					UART3SendString((u8*)TX,strlen((char*)TX));
					QuJianMa3PhoneNum=0;
				}
				/*�ر�4�Ź��ţ��ж����������ֻ����Ƿ���4�Ź�����ͬ*//////////////////////////////////////////////////////////////////////////								
				if(JD4!=0&&JD4==90&&JD2==0&&JD3==0&&JD1==0)///////////////////////////////////////////////////////
				{
					JD1=0;JD2=0;JD3=0;JD4=0;
					Servo_SetAngle1(JD1);
					Servo_SetAngle2(JD2);
					Servo_SetAngle3(JD3);
					Servo_SetAngle4(JD4);
//					QuJianMa4=0;
					if(QuJianMa1PhoneNum==QuJianMa4PhoneNum&&QuJianMa4PhoneNum==QuJianMa2PhoneNum&&QuJianMa4PhoneNum==QuJianMa3PhoneNum&&QuJianMa4PhoneNum!=0)
					{
						Usart_SendString(USART1,"HaiYouJian");
//						QuJianMa1PhoneNum=0;QuJianMa2PhoneNum=0;QuJianMa3PhoneNum=0;QuJianMa4PhoneNum=0;
						OLED_ClearArea(0,32,128,32);
						OLED_ShowChinese(16,32,"������δȡ��");
						OLED_ShowChinese(32,48,"�Ƿ�ȡ��");
						OLED_Update();
						K=4123;
						sprintf(TX,"K:%d,%lld,%lld,%lld\r\n",K,QuJianMa1PhoneNum,QuJianMa2PhoneNum,QuJianMa3PhoneNum);
						UART3SendString((u8*)TX,strlen((char*)TX));
					}
					else if(QuJianMa1PhoneNum==QuJianMa4PhoneNum&&QuJianMa4PhoneNum==QuJianMa2PhoneNum&&QuJianMa4PhoneNum!=0)
					{
						Usart_SendString(USART1,"HaiYouJian");
//						QuJianMa1PhoneNum=0;QuJianMa2PhoneNum=0;QuJianMa3PhoneNum=0;
						OLED_ClearArea(0,32,128,32);
						OLED_ShowChinese(16,32,"������δȡ��");
						OLED_ShowChinese(32,48,"�Ƿ�ȡ��");
						OLED_Update();
						K=412;
						sprintf(TX,"K:%d,%lld,%lld,%lld\r\n",K,QuJianMa1PhoneNum,QuJianMa2PhoneNum,QuJianMa4PhoneNum);
						UART3SendString((u8*)TX,strlen((char*)TX));
					}
					else if(QuJianMa1PhoneNum==QuJianMa4PhoneNum&&QuJianMa4PhoneNum==QuJianMa3PhoneNum&&QuJianMa4PhoneNum!=0)
					{
						Usart_SendString(USART1,"HaiYouJian");
//						QuJianMa2PhoneNum=0;QuJianMa4PhoneNum=0;QuJianMa3PhoneNum=0;
						OLED_ClearArea(0,32,128,32);
						OLED_ShowChinese(16,32,"������δȡ��");
						OLED_ShowChinese(32,48,"�Ƿ�ȡ��");
						OLED_Update();
						K=413;
						sprintf(TX,"K:%d\r\n",K);
						UART3SendString((u8*)TX,strlen((char*)TX));
					}
					else if(QuJianMa4PhoneNum==QuJianMa3PhoneNum&&QuJianMa4PhoneNum==QuJianMa2PhoneNum&&QuJianMa4PhoneNum!=0)
					{
						Usart_SendString(USART1,"HaiYouJian");
//						QuJianMa1PhoneNum=0;QuJianMa2PhoneNum=0;QuJianMa4PhoneNum=0;
						OLED_ClearArea(0,32,128,32);
						OLED_ShowChinese(16,32,"������δȡ��");
						OLED_ShowChinese(32,48,"�Ƿ�ȡ��");
						OLED_Update();
						K=423;
						sprintf(TX,"K:%d,%lld,%lld,%lld\r\n",K,QuJianMa2PhoneNum,QuJianMa3PhoneNum,QuJianMa4PhoneNum);
						UART3SendString((u8*)TX,strlen((char*)TX));
					}
					else if(QuJianMa4PhoneNum==QuJianMa1PhoneNum&&QuJianMa4PhoneNum!=QuJianMa2PhoneNum&&QuJianMa4PhoneNum!=QuJianMa3PhoneNum&&QuJianMa4PhoneNum!=0)
					{
						Usart_SendString(USART1,"HaiYouJian");
//						QuJianMa1PhoneNum=0;QuJianMa2PhoneNum=0;
						OLED_ClearArea(0,32,128,32);
						OLED_ShowChinese(16,32,"������δȡ��");
						OLED_ShowChinese(32,48,"�Ƿ�ȡ��");
						OLED_Update();
						Sum=QuJianMa1;
						sprintf(TX,"ȡ����1:%lld\r\n",QuJianMa1);
						UART3SendString((u8*)TX,strlen((char*)TX));
						
					}
					else if(QuJianMa4PhoneNum==QuJianMa2PhoneNum&&QuJianMa4PhoneNum!=QuJianMa3PhoneNum&&QuJianMa4PhoneNum!=QuJianMa1PhoneNum&&QuJianMa4PhoneNum!=0)
					{
						Usart_SendString(USART1,"HaiYouJian");
//						QuJianMa2PhoneNum=0;QuJianMa3PhoneNum=0;
						OLED_ClearArea(0,32,128,32);
						OLED_ShowChinese(16,32,"������δȡ��");
						OLED_ShowChinese(32,48,"�Ƿ�ȡ��");
						OLED_Update();
						Sum=QuJianMa2;
						sprintf(TX,"ȡ����2:%lld\r\n",QuJianMa2);
						UART3SendString((u8*)TX,strlen((char*)TX));
					}
					else if(QuJianMa4PhoneNum==QuJianMa3PhoneNum&&QuJianMa4PhoneNum!=QuJianMa1PhoneNum&&QuJianMa4PhoneNum!=QuJianMa2PhoneNum&&QuJianMa4PhoneNum!=0)
					{
						Usart_SendString(USART1,"HaiYouJian");
//						QuJianMa2PhoneNum=0;QuJianMa4PhoneNum=0;
						OLED_ClearArea(0,32,128,32);
						OLED_ShowChinese(16,32,"������δȡ��");
						OLED_ShowChinese(32,48,"�Ƿ�ȡ��");
						OLED_Update();
						Sum=QuJianMa3;
						sprintf(TX,"ȡ����3:%lld\r\n",QuJianMa3);
						UART3SendString((u8*)TX,strlen((char*)TX));
					}
					else if(QuJianMa3PhoneNum!=QuJianMa4PhoneNum&&QuJianMa2PhoneNum!=QuJianMa4PhoneNum&&QuJianMa1PhoneNum!=QuJianMa4PhoneNum&&QuJianMa4PhoneNum!=0)
					{
						Usart_SendString(USART1,"GUI_OFF");
						OLED_ClearArea(0,32,128,32);
						OLED_ShowChinese(24,32,"�����ѹر�");
						OLED_Update();
						delay_ms(1500);
						OLED_ClearArea(0,32,128,32);
						OLED_ClearArea(0,16,128,16);
						OLED_Update();
						Sum=0; 
						Show_Num=-8;
					}
//					sprintf(TX,"�ֻ���1:%lld �ֻ���2:%lld\r\n",QuJianMa1PhoneNum,QuJianMa2PhoneNum);
					UART3SendString((u8*)TX,strlen((char*)TX));
					QuJianMa4PhoneNum=0;
				}
				sprintf(TX,"JD1:%d JD2:%d JD3:%d JD4:%d\r\nGUI1:%d GUI2:%d GUI3:%d GUI4:%d\r\nK=%d\r\n",JD1,JD2,JD3,JD4,GUI1,GUI2,GUI3,GUI4,K);
				UART3SendString((u8*)TX,strlen((char*)TX));
				KeyNum=!15;
				
			}
		}
			OLED_Update();
	}
}
void my_testC4(void)//���Ա��֤���
{
	UART3SendString((u8*)"��ǰҳ��Ϊ���Ա�����֤ҳ��\r\n��ʾ�����Ա�����֤\r\n",
	strlen((char*)"��ǰҳ��Ϊ���Ա�����֤ҳ��\r\n��ʾ�����Ա�����֤\r\n"));
	OLED_ShowChinese(0,0,"���Ա�����֤��");	
	OLED_Update();
	KeyNum=0;
	unsigned long long Sum1=0; 
	int Show_Num=24;
	while(1)
	{
		KEY_Scan(&KeyNum);//��ȡ��ֵ
		
		if(KeyNum) 
		{
			if(KeyNum==10) 
			{
				KeyNum=0;
			}
			if(KeyNum>=0&&KeyNum<=10) //�������֣������䱣������Sum1
			{							
				Show_Num+=8;									//��ʾ����8�����ӣ�����ռ8�����أ�
				Sum1=Sum1*10+KeyNum;								//ͨ���۳�ʵ�ֱ��������ֵ
				OLED_ShowNum(Show_Num, 16,KeyNum, 1,8);
				OLED_Update();
				KeyNum=0;
			}
			if(KeyNum==12&&Show_Num>24)//ɾ��
			{
				Show_Num-=8;									//��ʾ����8�����ӣ�����ռ8�����أ�
				Sum1=(Sum1/10)-((Sum1%10)/10);								//ͨ���۳�ʵ�ֱ��������ֵ
				OLED_ShowString(Show_Num+8,16," ",8);
				OLED_Update();
				KeyNum=0;
			}
			if(KeyNum==16)//�˳�
			{
				OLED_Clear();
				return;
				
			}
			if(KeyNum==11)//ȷ��
			{
				if(Sum1==123456)
				{
					OLED_Clear();
					return;						
				}
				if(Sum1!=123456)
				{
					Usart_SendString(USART1,"YanZhengCuoWu");//����ʹ��
					OLED_ShowChinese(32,32,"��֤����");	
					Sum1=0;//����֤��
					Show_Num=24;//��ʾλ��
					OLED_ClearArea(0,16,128,16);
					OLED_Update();	
					delay_ms(1000);
					delay_ms(500);
					OLED_ClearArea(0,32,128,16);
					OLED_Update();
				}
				KeyNum=0;
				
			}
		}
		OLED_Update();
	}
   
	
}
void my_testD5(void)//��ݹ�ѡ�񣬼�ͷ���ϣ����
{
	UART3SendString((u8*)"��ǰҳ��Ϊѡ���ݹ�����ҳ���\r\n��ʾ�������С����\r\n",
	strlen((char*)"��ǰҳ��Ϊѡ���ݹ�����ҳ���\r\n��ʾ�������С����\r\n"));
	OLED_ShowChinese(0,0,"ѡ���ݹ�����");
	OLED_ShowChinese(16,16,"�����");
	OLED_ShowChinese(16,32,"С����");
	OLED_ShowImage(0,16,16,16,JianTou);
	OLED_ShowString(0,32,"  ",8);
	if(GUI1==1&&GUI2==1)
	{
		OLED_ShowChinese(64,16,"��������");
		OLED_ShowString(64,32,"    ",8);
	}
	if(GUI3==1&&GUI4==1)
	{
		OLED_ShowChinese(64,32,"��������");
		OLED_ShowString(64,16,"    ",8);
	}
	if(GUI3==1&&GUI4==1&&GUI1==1&&GUI2==1)
	{
		OLED_ShowChinese(64,32,"��������");
		OLED_ShowChinese(64,16,"��������");
	}
	OLED_Update();

}
void my_testD6(void)//��ݹ�ѡ�񣬼�ͷ���£�С��
{
	UART3SendString((u8*)"��ǰҳ��Ϊѡ���ݹ�����ҳ���\r\n��ʾ�������С����\r\n",
	strlen((char*)"��ǰҳ��Ϊѡ���ݹ�����ҳ���\r\n��ʾ�������С����\r\n"));
	OLED_ShowChinese(0,0,"ѡ���ݹ�����");
	OLED_ShowChinese(16,16,"�����");
	OLED_ShowChinese(16,32,"С����");
	OLED_ShowImage(0,32,16,16,JianTou);
	OLED_ShowString(0,16,"  ",8);
	if(GUI1==1&&GUI2==1)
	{
		OLED_ShowChinese(64,16,"��������");
		OLED_ShowString(64,32,"    ",8);
	}
	if(GUI3==1&&GUI4==1)
	{
		OLED_ShowChinese(64,32,"��������");
		OLED_ShowString(64,16,"    ",8);
	}
	if(GUI3==1&&GUI4==1&&GUI1==1&&GUI2==1)
	{
		OLED_ShowChinese(64,32,"��������");
		OLED_ShowChinese(64,16,"��������");
	}
	OLED_Update();

}
void my_testE7(void)//�����
{
	UART3SendString((u8*)"��ǰҳ��Ϊ�����ҳ��\r\n��ʾ�������š�ʣ������\r\n",
	strlen((char*)"��ǰҳ��Ϊ�����ҳ��\r\n��ʾ�������š�ʣ������\r\n"));
	OLED_Clear();
	OLED_ShowChinese(0,0,"������");
	OLED_ShowChinese(0,16,"ʣ������");
	OLED_ShowString(64,0,":",8);
	OLED_ShowString(64,16,":",8);
	OLED_ShowChinese(0,32,"ȡ�����ֻ��ţ�");
	OLED_ShowString(96,32,":",8);
	if(GUI1==0)
	{
		OLED_ShowNum(72,0,1,1,8);
	}
	else if(GUI2==0)
	{
		OLED_ShowNum(72,0,2,1,8);
	}
	OLED_ShowNum(72,16,2-(GUI1+GUI2),1,8);
	OLED_Update();
	int KeyNumD=0;
	int JD1=0,JD2=0;
char phone_center[100]= "0891683110709105F0";//����õĶ�������
	int Show_Num=-8;
	unsigned long long Sum=0;
	while(1)
	{
		KEY_Scan(&KeyNumD);
		
		if(KeyNumD) 
		{
			OLED_ShowChinese(0,0,"������");
			OLED_ShowChinese(0,16,"ʣ������");
			OLED_ShowString(64,0,":",8);
			OLED_ShowString(64,16,":",8);
			OLED_ShowChinese(0,32,"ȡ�����ֻ��ţ�");
			OLED_ShowString(96,32,":",8);
			if(GUI1==0)
			{
				OLED_ShowNum(72,0,1,1,8);
			}
			else if(GUI2==0)
			{
				OLED_ShowNum(72,0,2,1,8);
			}
			OLED_ShowNum(72,16,2-(GUI1+GUI2),1,8);
			OLED_Update();
			if(KeyNumD==10) 
			{
				KeyNumD=0;
			}
			if(KeyNumD>=0&&KeyNumD<=10) //�������֣������浽Sum
			{							
				Show_Num+=8;									//��ʾ����8�����ӣ�����ռ8�����أ�
				Sum=Sum*10+KeyNumD;								//ͨ���۳�ʵ�ֱ��������ֵ
				OLED_ShowNum(Show_Num, 48,KeyNumD, 1,8);
				OLED_Update();
				KeyNumD=0;
			}
			if(KeyNumD==12&&Show_Num>-8)//ɾ��,Show_Num>-8��Ϊ��ɾ����ͷ֮��ֹͣɾ��
			{
				Show_Num-=8;									//��ʾ����8�����ӣ�����ռ8�����أ�
				Sum=(Sum/10)-((Sum%10)/10);					//ͨ���۳�ʵ�ֱ��������ֵ
				OLED_ShowString(Show_Num+8,48," ",8);
				OLED_Update();
				KeyNumD=0;
			}
			if(KeyNumD==16)//�˳�
			{
				if(JD1==0&&JD2==0)
				{
					OLED_Clear();
					KeyNum=11;
					
//					OLED_Update();
					return;
				}
			}
			if(KeyNumD==11)//ȷ�Ϸ��Ͷ���
			{
				if(Sum>=10000000000)//�ж��ֻ��Ÿ�ʽ�Ƿ���ȷ
				{
					JD1=0;
					JD2=0;
					sprintf(phone_Num,"%lld",Sum);//������ĺ���Ž�����phone_Num
					PhoneNum(phone_Num, phone_NumOK);//����������phone_Num���ԭʼ����������86��F����ż����
					if(GUI1==0&&Sum!=0)
					{
						Usart_SendString(USART1,"GUI1_ON");
						JD1=90;
						MSGBZ=1;//����1�Ź���Ϣʹ��
						Servo_SetAngle1(JD1);
						SuiJiShu();//����ȡ�������SJS
						QuJianMa1=SJS;
						if((QuJianMa1==QuJianMa3)||(QuJianMa1==QuJianMa4)||(QuJianMa1==QuJianMa2))//��ֹȡ�����ظ�
						{
							SuiJiShuBY();//����ȡ�������SJSBY������
							QuJianMa1=SJSBY;
						}
						sprintf(TX,"ȡ����1:%lld\r\n ",QuJianMa1);
						UART3SendString((u8*)TX,strlen((char*)TX));
						QuJianMa1PhoneNum=Sum;//��һ�Ź��ֻ��Ŵ�����
						sprintf(QJM,"%lld",QuJianMa1);//��ȡ����Ž�����QJM
						ASCII_TO_Unicode(QJM,(char*)QJMUnicode);//��ȡ����ת��ΪUnicode
						UART3SendString((u8*)QJM,strlen((char*)QJM));
						sprintf(TX,"ȡ����1:%lld �ֻ���1:%lld\r\n ",QuJianMa1,QuJianMa1PhoneNum);
						UART3SendString((u8*)TX,strlen((char*)TX));
					}
					else if(GUI2==0&&GUI1==1&&Sum!=0)
					{
						Usart_SendString(USART1,"GUI2_ON");
						JD2=90;
						MSGBZ=2;//����2�Ź���Ϣ
						Servo_SetAngle2(JD2);
						SuiJiShu();
						QuJianMa2=SJS;
						
						if((QuJianMa2==QuJianMa1)||(QuJianMa2==QuJianMa4)||(QuJianMa2==QuJianMa3))
						{
							SuiJiShuBY();
							QuJianMa2=SJSBY;
						}
						QuJianMa2PhoneNum=Sum;
						sprintf(QJM,"%lld",QuJianMa2);
						ASCII_TO_Unicode(QJM,(char*)QJMUnicode);//��ȡ����ת��ΪUnicode
						sprintf(TX,"ȡ����2:%lld �ֻ���2:%lld\r\n ",QuJianMa2,QuJianMa2PhoneNum);
						UART3SendString((u8*)TX,strlen((char*)TX));
					}
					sprintf(TX,"JD1:%d JD2:%d\r\n",JD1,JD2/*,JD3,JD4*/);
					UART3SendString((u8*)TX,strlen((char*)TX));
//					UART3SendString((u8*)QJMUnicode,strlen((char*)QJMUnicode));
//					UART3SendString((u8*)phone_center,strlen((char*)phone_center));
					res=PUD_AIR780_SendChMessage((u8*)phone_center);//������Ϣ
					OLED_Update();
					sprintf(TX,"res:%d\r\n",res);
					UART3SendString((u8*)TX,strlen((char*)TX));
					if(res==1)
					{
						OLED_ClearArea(0,48,128,16);
						OLED_ShowChinese(0,48,"���ŷ��ͳɹ�");
						OLED_Update();
						
					}
					else
					{
						OLED_ClearArea(0,48,128,16);
						OLED_ShowChinese(0,48,"���ŷ���ʧ��");
						OLED_Update();
						delay_ms(1500);
						delay_ms(500);
						OLED_ClearArea(0,48,128,16);
						Sum=0;
						Show_Num=-8;
					}
				}			
				else
				{
					Usart_SendString(USART1,"HaoMaCuoWu");
					OLED_ClearArea(0,48,128,16);
					OLED_ShowChinese(0,48,"�������");
					OLED_Update();
					delay_ms(1500);
					delay_ms(500);
					OLED_ClearArea(0,48,128,16);
					Sum=0;
					Show_Num=-8;
				}
				sprintf(TX,"ȡ����1:%lld,ȡ����2:%lld,ȡ����3:%lld,ȡ����4:%lld\r\n",QuJianMa1,QuJianMa2,QuJianMa3,QuJianMa4);
				UART3SendString((u8*)TX,strlen((char*)TX));
				
				KeyNumD=!11;
			}
			if(KeyNumD==15)//����
			{
				if(JD1!=0&&JD1==90)
				{
					JD1=0;
					GUI1=1;
					Usart_SendString(USART1,"GUI1_OFF");//������ģ�鷢��1�Ź����ѹر�
					OLED_ShowNum(72,0,2,1,8);
					OLED_ShowNum(72,16,1,1,8);
					Servo_SetAngle1(JD1);
					GUI1_LED=1;//������
//					LED1_ON();
					
				}
				if(JD2!=0&&JD2==90)
				{
					JD2=0;
					GUI2=1;
					Usart_SendString(USART1,"GUI2_OFF");
					OLED_ShowChinese(72,0,"��");
					OLED_ShowNum(72,16,0,1,8);
					Servo_SetAngle2(JD2);
					OLED_Update();
					delay_ms(1000);
					OLED_Clear();
					GUI2_LED=1;
					KeyNum=11;
					Show_Num=-8;
					Sum=0;
					return ;
					
				}
				OLED_ClearArea(0,48,100,16);
				OLED_Update();
				Show_Num=-8;
				Sum=0;
				sprintf(TX,"ȡ����1%lld JD1:%d JD2:%d\r\nGUI1:%d GUI2:%d\r\n",QuJianMa1,JD1,JD2,GUI1,GUI2);
				UART3SendString((u8*)TX,strlen((char*)TX));
				sprintf(TX,"ȡ����1:%lld,ȡ����2:%lld,ȡ����3:%lld,ȡ����4:%lld\r\n",QuJianMa1,QuJianMa2,QuJianMa3,QuJianMa4);
				UART3SendString((u8*)TX,strlen((char*)TX));
				KeyNumD=0;
			}
		}
		OLED_Update();
		
	}
	
	

}
void my_testE8(void)//С����
{
	UART3SendString((u8*)"��ǰҳ��ΪС����ҳ��\r\n��ʾ�������š�ʣ������\r\n",
	strlen((char*)"��ǰҳ��ΪС����ҳ��\r\n��ʾ�������š�ʣ������\r\n"));
	OLED_Clear();
	OLED_ShowChinese(0,0,"������");
	OLED_ShowChinese(0,16,"ʣ������");
	OLED_ShowString(64,0,":",8);
	OLED_ShowString(64,16,":",8);
	OLED_ShowChinese(0,32,"ȡ�����ֻ��ţ�");
	OLED_ShowString(96,32,":",8);
	if(GUI3==0)
	{
		OLED_ShowNum(72,0,3,1,8);
	}
	else if(GUI4==0)
	{
		OLED_ShowNum(72,0,4,1,8);
	}
	OLED_ShowNum(72,16,2-(GUI3+GUI4),1,8);
	OLED_Update();
	int KeyNumD=0;
	int JD3=0,JD4=0;
char phone_center[100]= "0891683110709105F0";
	int Show_Num=-8;
	unsigned long long Sum=0;
	while(1)
	{
		KEY_Scan(&KeyNumD);
		if(KeyNumD) 
		{
			OLED_ShowChinese(0,0,"������");
			OLED_ShowChinese(0,16,"ʣ������");
			OLED_ShowString(64,0,":",8);
			OLED_ShowString(64,16,":",8);
			OLED_ShowChinese(0,32,"ȡ�����ֻ��ţ�");
			OLED_ShowString(96,32,":",8);
			if(GUI3==0)
			{
				OLED_ShowNum(72,0,3,1,8);
			}
			else if(GUI4==0)
			{
				OLED_ShowNum(72,0,4,1,8);
			}
			OLED_ShowNum(72,16,2-(GUI3+GUI4),1,8);
			OLED_Update();
			if(KeyNumD==10) 
			{
				KeyNumD=0;
			}
			if(KeyNumD>=0&&KeyNumD<=10) 
			{							
				Show_Num+=8;									//��ʾ����8�����ӣ�����ռ8�����أ�
				Sum=Sum*10+KeyNumD;								//ͨ���۳�ʵ�ֱ��������ֵ
				OLED_ShowNum(Show_Num, 48,KeyNumD, 1,8);
				OLED_Update();
				KeyNumD=0;
			}
			if(KeyNumD==12&&Show_Num>-8)
			{
				Show_Num-=8;									//��ʾ����8�����ӣ�����ռ8�����أ�
				Sum=(Sum/10)-((Sum%10)/10);								//ͨ���۳�ʵ�ֱ��������ֵ
				OLED_ShowString(Show_Num+8,48," ",8);
				OLED_Update();
				KeyNumD=0;
			}
			if(KeyNumD==16)//�˳�
			{
				if(JD3==0&&JD4==0)
				{
					OLED_Clear();
					KeyNum=11;
					
//					OLED_Update();
					return;
				}

				
			}
			if(KeyNumD==11)//ȷ��
			{
				
				
				if(Sum>=10000000000)
				{
					JD3=0;
					JD4=0;
					
					sprintf(phone_Num,"%lld",Sum);
					PhoneNum(phone_Num, phone_NumOK);
//					UART3SendString((u8*)phone_center,strlen((char*)phone_center));
					if(GUI3==0&&Sum!=0)
					{
						Usart_SendString(USART1,"GUI3_ON");
						
						JD3=90;
						MSGBZ=3;
						Servo_SetAngle3(JD3);
						
						SuiJiShu();
						QuJianMa3=SJS;
						if((QuJianMa3==QuJianMa1)||(QuJianMa3==QuJianMa2))
						{
							SuiJiShuBY();
							QuJianMa3=SJSBY;
						}
						sprintf(TX,"ȡ����3:%lld\r\n ",QuJianMa3);
						UART3SendString((u8*)TX,strlen((char*)TX));
						QuJianMa3PhoneNum=Sum;
						sprintf(QJM,"%lld",QuJianMa3);
						ASCII_TO_Unicode(QJM,(char*)QJMUnicode);//��ȡ����ת��ΪUnicode
						UART3SendString((u8*)QJM,strlen((char*)QJM));
						sprintf(TX,"ȡ����3:%lld �ֻ���3:%lld\r\n ",QuJianMa3,QuJianMa3PhoneNum);
						UART3SendString((u8*)TX,strlen((char*)TX));
						
					}
					else if(GUI4==0&&GUI3==1&&Sum!=0)
					{
						Usart_SendString(USART1,"GUI4_ON");
						JD4=90;
						MSGBZ=4;
						Servo_SetAngle4(JD4);
						SuiJiShu();
						QuJianMa4=SJS;
						if((QuJianMa4==QuJianMa1)||(QuJianMa4==QuJianMa2)||(QuJianMa4==QuJianMa3))
						{
							SuiJiShuBY();
							QuJianMa4=SJSBY;
						}
						QuJianMa4PhoneNum=Sum;
						sprintf(QJM,"%lld",QuJianMa4);
						ASCII_TO_Unicode(QJM,(char*)QJMUnicode);//��ȡ����ת��ΪUnicode
						sprintf(TX,"ȡ����4:%lld �ֻ���4:%lld\r\n ",QuJianMa4,QuJianMa4PhoneNum);
						UART3SendString((u8*)TX,strlen((char*)TX));
					}
					sprintf(TX,"JD3:%d JD4:%d\r\n",JD3,JD4/*,JD3,JD4*/);
					UART3SendString((u8*)TX,strlen((char*)TX));
//					UART3SendString((u8*)QJMUnicode,strlen((char*)QJMUnicode));
//					UART3SendString((u8*)phone_center,strlen((char*)phone_center));
					res=PUD_AIR780_SendChMessage((u8*)phone_center);//������Ϣ
					OLED_Update();
					sprintf(TX,"res:%d\r\n",res);
					UART3SendString((u8*)TX,strlen((char*)TX));
					if(res==1)
					{
						OLED_ClearArea(0,48,128,16);
						OLED_ShowChinese(0,48,"���ŷ��ͳɹ�");
						OLED_Update();
						
					}
					else
					{
						OLED_ClearArea(0,48,128,16);
						OLED_ShowChinese(0,48,"���ŷ���ʧ��");
						OLED_Update();
						delay_ms(1500);
						delay_ms(500);
						OLED_ClearArea(0,48,128,16);
						Sum=0;
						Show_Num=-8;
					}
				}			
				else
				{
					OLED_ClearArea(0,48,128,16);
					OLED_ShowChinese(0,48,"�������");
					OLED_Update();
					delay_ms(1500);
					delay_ms(500);
					OLED_ClearArea(0,48,128,16);
					Sum=0;
					Show_Num=-8;
				}
				sprintf(TX,"ȡ����1:%lld,ȡ����2:%lld,ȡ����3:%lld,ȡ����4:%lld\r\n",QuJianMa1,QuJianMa2,QuJianMa3,QuJianMa4);
				UART3SendString((u8*)TX,strlen((char*)TX));
				
				KeyNumD=!11;
			}
			if(KeyNumD==15)//����
			{
				if(JD3!=0&&JD3==90)
				{
					JD3=0;
					GUI3=1;
					Usart_SendString(USART1,"GUI3_OFF");
					OLED_ShowNum(72,0,4,1,8);
					OLED_ShowNum(72,16,1,1,8);
					Servo_SetAngle3(JD3);
					OLED_Update();
					GUI3_LED=1;
				}
				if(JD4!=0&&JD4==90)
				{
					JD4=0;
					GUI4=1;
					Usart_SendString(USART1,"GUI4_OFF");
					OLED_ShowChinese(72,0,"��");
					OLED_ShowNum(72,16,0,1,8);
					Servo_SetAngle4(JD4);
					OLED_Update();
					delay_ms(1000);
					OLED_Clear();
					GUI4_LED=1;
					KeyNum=11;
					Show_Num=-8;
					Sum=0;
					return ;
					
				}
				OLED_ClearArea(0,48,100,16);
				OLED_Update();
				Show_Num=-8;
				Sum=0;
				sprintf(TX,"ȡ����1%lld JD1:%d JD2:%d\r\nGUI1:%d GUI2:%d\r\n",QuJianMa1,JD3,JD4,GUI3,GUI4);
				UART3SendString((u8*)TX,strlen((char*)TX));
				sprintf(TX,"ȡ����1:%lld,ȡ����2:%lld,ȡ����3:%lld,ȡ����4:%lld\r\n",QuJianMa1,QuJianMa2,QuJianMa3,QuJianMa4);
				UART3SendString((u8*)TX,strlen((char*)TX));
				KeyNumD=!15;
			}
		}
		OLED_Update();
		
	}
	

}


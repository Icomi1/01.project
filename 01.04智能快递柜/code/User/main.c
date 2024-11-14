#include "sys.h"
#include "stm32f10x_conf.h"
#include "delay.h"
#include "menu.h"
#include "LED.h"
#include "OLED.h"
#include "usart2.h"
#include "usart.h"
#include "delay.h"
#include "Timer.h"
#include "sys.h"
#include "string.h"
#include "OLED.h"
#include "ASCII_TO_Unicode.h"
#include "SuiJiShu.h"
#include "stdio.h"
#include "key.h"
#include "AIR780.h"
#include "KEYPAD4x4.h"
#include "Servo.h"
#include "Timer.h"
#include "Phone_Num.h"
#include "DS1302.h"
char PhoneUnicode[100];
char QuJianMa1_1[100];
char QuJianMa1Unicode[100];
char phone_number1[100];
char phone_number2[100]= "15586441755";
uint32_t SJS;
uint32_t SJSBY;
int GUI1_LED=0;
int GUI2_LED=0;
int GUI3_LED=0;
int GUI4_LED=0;

/*PUDģʽ����*/
//�������ĺ������Ϊ�ܷ��͵�����
char phone_Num[100];//���봦��֮ǰ��ԭʼ����
char phone_NumOK[100];
char MSG[100]= "07915185461457F521000D91685185461457F50008044F60597D";//����
char MSG1[100]= "07915185461457F521000D91685185461457F50008044F60597D";//1�ſ�ݹ����
char MSG2[100]= "07915185461457F521000D91685185461457F50008044F60597D";//2�ſ�ݹ����
char MSG3[100]= "07915185461457F521000D91685185461457F50008044F60597D";//3�ſ�ݹ����
char MSG4[100]= "07915185461457F521000D91685185461457F50008044F60597D";//4�ſ�ݹ����
char QJM[10];//
char QJMUnicode[10];//
char XH[10];//
char XHUnicode[10];//
/*ʱ��ģ�����*/
uint8_t D_Buf[11]={0};  //�洢������
uint8_t Time_Buf[10]={0}; //�洢ʱ����
uint8_t year=20;           //�����λ
uint8_t week[2]={0};               //����
uint8_t KEY0_Num=0;           //�л�������
//////////////////////////////////////////////////////////
//uint64_t sjh=1000;
int RES,RETURN;
char qhmUnicode[100];
char qhm[100];
uint64_t QuJianMa1,QuJianMa2,QuJianMa3,QuJianMa4;

int KeyNum=0;//��ֵ
unsigned char  func_index =0;//������־��ֵ����ִ�еĺ�������Ԫ��
unsigned char last_index =10;//ִ�������֮��ı�־

typedef struct//����ṹ��key_table
{
	unsigned char current;//��������ҳ�����	
	unsigned char down;//���·�
	unsigned char enter;//���빦�ܣ�ȷ��
	void (*current_operation)();//ִ�к�������ʾͼƬ
}key_table;
//����һ������ 9 �� key_table �ṹ��Ԫ�ص����� table
key_table table[9]=
{
//��ҳ
	{0,0,1,(*my_testA0)},//��������ҳ

	{1,2,3,(*my_testB1)},//���ܿ�ݹ����ϵͳҳ����ͷ���ϣ�ȡ����ȡ��
	{2,1,4,(*my_testB2)},//���ܿ�ݹ����ϵͳҳ����ͷ���£����Ա���

	{3,3,3,(*my_testC3)},//������ȡ����
	{4,4,5,(*my_testC4)},//���Ա��֤���

	{5,6,7,(*my_testD5)},//ѡ���ݹ����ͣ���ͷ���ϣ����
	{6,5,8,(*my_testD6)},//ѡ���ݹ����ͣ���ͷ���£�С��
	{7,7,5,(*my_testE7)},//�����ţ�1�Ź��2�Ź񣬴��
	{8,8,5,(*my_testE8)},//�����ţ�3�Ź��4�Ź�С��
};
void (*current_operation_index)();//���浱ǰҪִ�еĺ���
int main()
{
	LED_Init();//LED��ʼ��
//Ds1302Init();//�ڵ�һ�����ú�ʱ��֮�����أ�Ȼ��֮��ע�͵�
	NVIC_Configuration(); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	OLED_Init();//oled��ʾ����ʼ��
	OLED_ShowChinese(16,16,"ϵͳ��ʼ����");
	OLED_Update();
	KEY_4x4_Init();//���󰴼���ʼ��
	OLED_Clear();//����
	Servo_Init();//�����ʼ��
//	Key_Init();//������ʼ��
//	TIM3_Int_Init();
	MyUSART_Init();//����1����ģ���ʼ���������ʹ̶�Ϊ9600
	USART2_Init(115200);	//����2 4Gͨ��ģ��
	USART3_Init(115200);	//����3 PC��
	RETURN=PUD_AIR_Dect();//4Gģ���ʼ��+��Ϣ��ʾ
	switch(RETURN)//��switch����ж�4Gģ���Ƿ��Լ�ɹ�����oled��ʾ
	{
		case AIR_OK:
		{
			OLED_ShowString(8,16,"4G",8);
			OLED_ShowChinese(24,16,"ģ���Լ�ɹ�");
			OLED_Update();
		}
		break;
		default:
		{
			OLED_ShowString(8,16,"4G",8);
			OLED_ShowChinese(24,16,"ģ���Լ�ʧ��");
			OLED_Update();
		}
	}
	delay_ms(1000);
	OLED_ShowChinese(8,16,"ϵͳ��ʼ�����");
	OLED_Update();
	UART3SendString((u8*)"��ʼ�����\r\n",strlen((char*)"��ʼ�����\r\n"));//�����Է��� ��ɳ�ʼ�� ����Ϣ
	delay_ms(1000);
	OLED_Clear();
	OLED_ShowImage(32,0,70,70,KuaiDi);//ͼ��
	OLED_Update();
	Timer_Init();//��ʱ����ʼ��
	Usart_SendString(USART1,"ASR_ON");//������ģ�鷢�͡�ASR_ON��
	while(1)
	{
		KEY_Scan(&KeyNum);//����ɨ�躯������ɨ���ֵд��KeyNum��
		if(KeyNum)
		{
			if(KeyNum==14)//���·�
         {
            func_index = table[func_index].down;    //
				KeyNum=0;
//            while(!KeyNum);//����ѭ��
         }
			if(KeyNum==11)//����/ȷ��
         {
             func_index = table[func_index].enter;    //
			 	KeyNum=0;
//            while(!KeyNum);
			}
			if(KeyNum==16)//����
			{
				func_index=0;
			}
			if (func_index != last_index)//�жϱ�־�Ƿ�ı䣬�ڲ���֮����ҳ��
			{
				//�洢��ǰҪִ�еĺ���//table[0].current_operation=my_testA0=current_operation_index
				current_operation_index = table[func_index].current_operation;//table[0].current_operation=*my_testA0
				if(((last_index-func_index)>=2)||((func_index-last_index)>=2)||last_index==0)//�ж��Ƿ�ص�������
				{
					OLED_Clear();
				}
				(*current_operation_index)();//ִ�к���
//				UART3SendString((u8*)"����\r\n",strlen((char*)"����\r\n"));//
				last_index = func_index;//
			} 	
	
		}
			
		
	}

}

/************************************************************************************
//�����������������ҡ����Ա�����: https://shop149406681.taobao.com/?spm=a1z10.1-c.0.0.64cb457fKzgKea 
//�Ա����������ؼ��ʣ�K_A37_005
//  �� �� ��   : v1.0
//  ��    ��   : ����������
//  ����Ⱥ :			 1041406448�����������������ע���棩
//    CSDN  ��������������Դ�����б����н���˵����   :��Ƭ��������˹�(�����ؼ��ʣ�K_A37_005)
//    BILIBILI����ʾ��Ƶ�� :����������(�����ؼ��ʣ�K_A37_005)
//  ��������   : 2021-6-21
//  ����޸�   : 2021-11-21
//  ��������   : ADS1115����Գ���
//  ��������   : STM32F103C8T6   ����8M  ϵͳʱ��72M
����
ADS1115--------------------------------STM32F103C8T6
VCC-- ---------------------------------5V
GND- ----------------------------------GND
SCL- ----------------------------------PB_4 //SCL
SDA- ----------------------------------PB_3 //SDA	

OLED0.96
VCC-- ---------------------------------3.3V
GND- ----------------------------------GND
SCL- ----------------------------------PB_8 //SCL
SDA- ----------------------------------PB_9 //SDA	
*************************************************************************************/

#include "DS1302.h"
#include "delay.h"
#include "oled.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"

//---DS1302д��Ͷ�ȡʱ����ĵ�ַ����---//
//---���ʱ�������� ���λ��дλ;-------//
uint8_t  READ_RTC_ADDR[7] = {0x81, 0x83, 0x85, 0x87, 0x89, 0x8b, 0x8d}; 
uint8_t  WRITE_RTC_ADDR[7] = {0x80, 0x82, 0x84, 0x86, 0x88, 0x8a, 0x8c};
extern uint8_t D_Buf[11];  //�洢������
extern uint8_t Time_Buf[10]; //�洢ʱ����
extern uint8_t year;           //�����λ
extern uint8_t week[2];               //����
extern uint8_t KEY0_Num;           //�л�������
u8 TIMELED1_S_NUM;
u8 TIMELED1_M_NUM;
u8 TIMELED1_H_NUM;
extern char TIMELED1_S[100];
extern char TIMELED1_M[100];
extern char TIMELED1_H[100];

//---DS1302ʱ�ӳ�ʼ��2013��1��1�����ڶ�12��00��00�롣---//
//---�洢˳�������ʱ��������,�洢��ʽ����BCD��---//
uint8_t TIME[7] = {0x20, 0x09, 0x21, 0x17, 0x10, 0x04, 0x24};

/*******************************************************************************
���ó��������ģʽ
*******************************************************************************/
void DS1302_OUT_GPIO_INIT(void)
{

	GPIO_InitTypeDef  GPIO_InitStructure;  

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_3 | GPIO_Pin_4| GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//
	GPIO_Init(GPIOB, &GPIO_InitStructure);

}
/*******************************************************************************
�����������ó���������ģʽ
*******************************************************************************/
void DS1302_IN_GPIO_INIT(void)//DAT
{
    GPIO_InitTypeDef  GPIO_InitStructure;  

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
void Delay1us(void)
{
    delay_us(1);
}
void Ds1302Write(uint8_t addr, uint8_t dat)
{
	uint8_t n;
    DS1302_OUT_GPIO_INIT();
	RST_L;
	Delay1us();
   
	SCLK_L;//�Ƚ�SCLK�õ͵�ƽ��
	Delay1us();
	RST_H; //Ȼ��RST(CE)�øߵ�ƽ��
	Delay1us();

	for (n=0; n<8; n++)//��ʼ���Ͱ�λ��ַ����
	{
        if(addr & 0x01)
        {
            DSIO_H;//���ݴӵ�λ��ʼ����
        }
        else
        {
            DSIO_L;
        }
		addr >>= 1;
		SCLK_H;//������������ʱ��DS1302��ȡ����
		Delay1us();
		SCLK_L;
		Delay1us();
	}
	for (n=0; n<8; n++)//д��8λ����
	{
		if(dat & 0x01)
        {
            DSIO_H;//���ݴӵ�λ��ʼ����
        }
        else
        {
            DSIO_L;
        }
		dat >>= 1;
		SCLK_H;//������������ʱ��DS1302��ȡ����
		Delay1us();
		SCLK_L;
		Delay1us();	
	}	
		 
	RST_L;//�������ݽ���
	Delay1us();
}

/*******************************************************************************
* �� �� ��         : Ds1302Read
* ��������		   : ��ȡһ����ַ������
* ��    ��         : addr
* ��    ��         : dat
*******************************************************************************/

uint8_t Ds1302Read(uint8_t addr)
{
	uint8_t n,dat,dat1;
	RST_L;
	Delay1us();

	SCLK_L;//�Ƚ�SCLK�õ͵�ƽ��
	Delay1us();
	RST_H;//Ȼ��RST(CE)�øߵ�ƽ��
	Delay1us();
    
	for(n=0; n<8; n++)//��ʼ���Ͱ�λ��ַ����
	{
		if(addr & 0x01)
        {
            DSIO_H;//���ݴӵ�λ��ʼ����
        }
        else
        {
            DSIO_L;
        }
		addr >>= 1;
		SCLK_H;//������������ʱ��DS1302��ȡ����
		Delay1us();
		SCLK_L;//DS1302�½���ʱ����������
		Delay1us();
	}
	Delay1us();
    DS1302_IN_GPIO_INIT();
	for(n=0; n<8; n++)//��ȡ8λ����
	{
		dat1 = GET_DSIO;//�����λ��ʼ����
		dat = (dat>>1) | (dat1<<7);
		SCLK_H;
		Delay1us();
		SCLK_L;//DS1302�½���ʱ����������
		Delay1us();
	}
    DS1302_OUT_GPIO_INIT();
	RST_L;
	Delay1us();	//����ΪDS1302��λ���ȶ�ʱ��,����ġ�
	SCLK_H;
	Delay1us();
	DSIO_L;
	Delay1us();
	DSIO_H;
	Delay1us();
	return dat;	
}

/*******************************************************************************
* �� �� ��         : Ds1302Init
* ��������		   : ��ʼ��DS1302.
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/

void Ds1302Init(void)
{
	uint8_t n;
	Ds1302Write(0x8E,0X00);		 //��ֹд���������ǹر�д��������
	for (n=0; n<7; n++)//д��7���ֽڵ�ʱ���źţ�����ʱ��������
	{
		Ds1302Write(WRITE_RTC_ADDR[n],TIME[n]);	
	}
	Ds1302Write(0x8E,0x80);		 //��д��������
}

/*******************************************************************************
* �� �� ��         : Ds1302ReadTime
* ��������		   : ��ȡʱ����Ϣ
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/

void Ds1302ReadTime(void)
{
	uint8_t n;
	for (n=0; n<7; n++)//��ȡ7���ֽڵ�ʱ���źţ�����ʱ��������
	{
		TIME[n] = Ds1302Read(READ_RTC_ADDR[n]);
	}
		
}
void DS1302_Dis()
{
 
		Ds1302ReadTime();
		Time_Buf[7]=TIME[0]%16+0x30;
		Time_Buf[6]=TIME[0]/16+0x30;
		Time_Buf[5]=':';
		/**����ʾ**/
		Time_Buf[4]=TIME[1]%16+0x30;
		Time_Buf[3]=TIME[1]/16+0x30;

		Time_Buf[2]=':';
		/**ʱ��ʾ**/
		Time_Buf[1]=TIME[2]%16+0x30;
		Time_Buf[0]=TIME[2]/16+0x30;
		Time_Buf[8]='\0';//��������
	
	
		/**����ʾ**/
		D_Buf[9]=TIME[3]%16+0x30;
		D_Buf[8]=TIME[3]/16+0x30;   
		D_Buf[7]='-';    
		/**����ʾ**/
		D_Buf[6]=TIME[4]%16+0x30;
		D_Buf[5]=TIME[4]/16+0x30;   
		D_Buf[4]='-';    
		/**�����λ��ʾ**/
		D_Buf[3]=TIME[6]%16+0x30;
		D_Buf[2]=TIME[6]/16+0x30;   
		/**��͸�λ��ʾ**/
		D_Buf[1]=year%10+0x30;
		D_Buf[0]=year/10+0x30;   

		D_Buf[10]='\0';//��������
		
		/**����ʾ**/
//		week[0]=TIME[5]+0x30;
		week[0]=TIME[5];
		week[1] = '\0';
		OLED_ShowString(24,0,(char*)D_Buf,8);
		OLED_ShowChinese(40,16,"����");
		switch(week[0])
		{
			case 1:OLED_ShowChinese(72,16,"һ");break;
			case 2:OLED_ShowChinese(72,16,"��");break;
			case 3:OLED_ShowChinese(72,16,"��");break;
			case 4:OLED_ShowChinese(72,16,"��");break;
			case 5:OLED_ShowChinese(72,16,"��");break;
			case 6:OLED_ShowChinese(72,16,"��");break;
			case 7:OLED_ShowChinese(72,16,"��");break;
			default:OLED_ShowChinese(72,16,"");
		}
//		OLED_ShowChinese();
//		OLED_P16x16Ch(72,2,week[0]);
		OLED_ShowString(30,32,(char*)Time_Buf,8);
		OLED_Update();
		
		
}
void DS1302_Dis1()
{
 
		Ds1302ReadTime();
		Time_Buf[7]=TIME[0]%16+0x30;
		Time_Buf[6]=TIME[0]/16+0x30;
		Time_Buf[5]=':';
		/**����ʾ**/
		Time_Buf[4]=TIME[1]%16+0x30;
		Time_Buf[3]=TIME[1]/16+0x30;

		Time_Buf[2]=':';
		/**ʱ��ʾ**/
		Time_Buf[1]=TIME[2]%16+0x30;
		Time_Buf[0]=TIME[2]/16+0x30;
		Time_Buf[8]='\0';//��������
	
	
		/**����ʾ**/
		D_Buf[9]=TIME[3]%16+0x30;
		D_Buf[8]=TIME[3]/16+0x30;   
		D_Buf[7]='-';    
		/**����ʾ**/
		D_Buf[6]=TIME[4]%16+0x30;
		D_Buf[5]=TIME[4]/16+0x30;   
		D_Buf[4]='-';    
		/**�����λ��ʾ**/
		D_Buf[3]=TIME[6]%16+0x30;
		D_Buf[2]=TIME[6]/16+0x30;   
		/**��͸�λ��ʾ**/
		D_Buf[1]=year%10+0x30;
		D_Buf[0]=year/10+0x30;   

		D_Buf[10]='\0';//��������
		
		/**����ʾ**/
//		week[0]=TIME[5]+0x30;
		week[0]=TIME[5];
		week[1] = '\0';
//		OLED_ShowString(24,0,(char*)D_Buf,8);
//		OLED_ShowChinese(40,16,"����");
//		switch(week[0])
//		{
//			case 1:OLED_ShowChinese(72,16,"һ");break;
//			case 2:OLED_ShowChinese(72,16,"��");break;
//			case 3:OLED_ShowChinese(72,16,"��");break;
//			case 4:OLED_ShowChinese(72,16,"��");break;
//			case 5:OLED_ShowChinese(72,16,"��");break;
//			case 6:OLED_ShowChinese(72,16,"��");break;
//			case 7:OLED_ShowChinese(72,16,"��");break;
//			default:OLED_ShowChinese(72,16,"");
//		}
////		OLED_ShowChinese();
////		OLED_P16x16Ch(72,2,week[0]);
		OLED_ShowString(0,5,(char*)Time_Buf,6);
		OLED_Update();
		
		
}
void DS1302_Dis2()
{
 
		Ds1302ReadTime();
		TIMELED1_S[1]=TIME[0]%16+0x30;
		TIMELED1_S[0]=TIME[0]/16+0x30;
//		TIMELED1_S[2]='\0';
		/**����ʾ**/
		TIMELED1_M[1]=TIME[1]%16+0x30;
		TIMELED1_M[0]=TIME[1]/16+0x30;
//	TIMELED1_M[2]='\0';
		/**ʱ��ʾ**/
		TIMELED1_H[1]=TIME[2]%16+0x30;
		TIMELED1_H[0]=TIME[2]/16+0x30;
//	TIMELED1_H[2]='\0';
		TIMELED1_S_NUM=atoi(TIMELED1_S);
		TIMELED1_M_NUM=atoi(TIMELED1_M);
		TIMELED1_H_NUM=atoi(TIMELED1_H);
}


































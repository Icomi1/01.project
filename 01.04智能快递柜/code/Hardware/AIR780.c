#include "stm32f10x.h"                  // Device header
#include "usart2.h" 
#include "stdio.h"
#include "string.h"
#include "Delay.h"
#include "AIR780.h"
#include "OLED.h"

#define Success 1U
#define Failure 0U
#define Uart1_Buf_Max 100 
u8  Rec_Buf[Uart1_Buf_Max];  //�������ݻ���

//u8 dtbuf[50];   								//��ӡ������	
//u8 Flag_Rec_Call=0;
unsigned long  Time_Cont = 0;//��ʱ��������
u16 point2 = 0;
extern int RES;
extern char phone_NumOK[100];
extern char MSG[100];//����
extern char MSG1[100];//����
extern char MSG2[100];//����
extern char MSG3[100];//����
extern char MSG4[100];//����
extern char QJMUnicode[10];//
extern int MSGBZ;
//void errorLog(int num);
//void Sys_Soft_Reset(void);
/*
void errorLog(int num)
{
	printf("ERROR%d\r\n",num);
	while (1)
	{
		if (sendCommand("AT\r\n", "OK\r\n", 100, 10) == Success)
		{
			Sys_Soft_Reset();
		}
		delay_ms(200);
	}
}*/
//void Sys_Soft_Reset(void)
//{  
//    SCB->AIRCR =0X05FA0000|(u32)0x04;      
//}
/*
void sendMessage(char *number,char *msg)
{
	char send_buf[40] = {0};
	memset(send_buf, 0, 40);    //���
	strcpy(send_buf, "AT+CMGS=\"");
	strcat(send_buf, number);
	strcat(send_buf, "\"\r\n");
	if (sendCommand(send_buf, ">", 3000, 10) == Success);
	else errorLog(7);

	if (sendCommandHex(msg, 3000, 10) == Success);
	else errorLog(8);

	memset(send_buf, 0, 40);    //���
	send_buf[0] = 0x1a;
	send_buf[1] = '\0';
	if (sendCommand(send_buf, "OK\r\n", 10000, 30) == Success);
	else errorLog(9);
}*/
/*
unsigned int sendCommand(char *Command, char *Response, unsigned long Timeout, unsigned char Retry)
{
	unsigned char n;
	USART2_CLR_Buf();
	for (n = 0; n < Retry; n++)
	{
		u2_printf(Command); 		//����GPRSָ��
		
		printf("\r\n***************send****************\r\n");
		printf(Command);
		
		Time_Cont = 0;
		while (Time_Cont < Timeout)
		{
			delay_ms(100);
			Time_Cont += 100;
			if (strstr((char*)USART2_RX_BUF, Response) != NULL)
			{				
				printf("\r\n***************receive****************\r\n");
				printf((char*)USART2_RX_BUF);
				USART2_CLR_Buf();
				return Success;
			}
			
		}
		Time_Cont = 0;
	}
	printf("\r\n***************receive****************\r\n");
	printf((char*)USART2_RX_BUF);
	USART2_CLR_Buf();
	return Failure;
}*/
void USART2_CLR_Buf(void)                           // ���ڻ�������
{
	memset(USART2_RX_BUF, 0, USART2_MAX_RECV_LEN);      //���
  point2 = 0;                    
}
unsigned char nibble2c(unsigned char c)
{
   if ((c>='0') && (c<='9'))
      return c-'0' ;
   if ((c>='A') && (c<='F'))
      return c+10-'A' ;
   if ((c>='a') && (c<='a'))
      return c+10-'a' ;
   return -1 ;
}
unsigned char hex2c(unsigned char c1, unsigned char c2)
{
   if(c1 >= 0)
     return c1*16 + c2 ;
   return 0 ;
}
unsigned int sendCommandHex(char *Command, unsigned long Timeout, unsigned char Retry)
{
	unsigned char n,i;
	unsigned char dat = 0,dat1 = 0,dat_hex = 0;
	char send_buf[30] = {0};
	memset(send_buf, 0, 30);    //���
	USART2_CLR_Buf();
	for (n = 0; n < Retry; n++)
	{
		for(i = 0 ; i < strlen(Command) ;i++)
		{
			
			if(i%2==0)
			{
				dat = nibble2c(Command[i]);	//��ȡ����λ������
			}
				
			if(i%2==1)
			{
				dat1 = nibble2c(Command[i]);	//��ȡż��λ������

				dat_hex = hex2c(dat,dat1);	//���ܳ�һ��HEX����

				send_buf[i/2]=dat_hex;	//ƴ�ӵ�send_buf���ȴ�����
			}
					
		}

		UART2SendString((u8*)send_buf,strlen(send_buf));
//		SendString(send_buf);

		Time_Cont = 0;
		while (Time_Cont < Timeout)
		{
			delay_ms(100);
			Time_Cont += 100;
			if (strstr((char*)Rec_Buf, send_buf) != NULL)
			{
				
				USART2_CLR_Buf();
				return Success;
			}
			
		}
		Time_Cont = 0;
	}
	
	USART2_CLR_Buf();
	return Failure;
}

///////////////////////////////////////////////////////////////////////////////////////////////
void AIR_at_response(u8 mode)
{
	if(USART2_RX_STA&0X8000)		//���յ�һ��������
	{ 
		USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//��ӽ�����
		printf("%s",USART2_RX_BUF);	//���͵�����
		if(mode)USART2_RX_STA=0;		
	} 
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//ATK-SIM900A �������(���Ų��ԡ����Ų��ԡ�GPRS����)���ô���
//sim900a���������,�����յ���Ӧ��
//str:�ڴ���Ӧ����
//����ֵ:0,û�еõ��ڴ���Ӧ����
//    ����,�ڴ�Ӧ������λ��(str��λ��)
u8* AIR780_check_cmd(u8 *str)
{
	char *strx=0;
	if(USART2_RX_STA&0X8000)		//���յ�һ��������
	{ 
		USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//��ӽ�����
		strx=strstr((const char*)USART2_RX_BUF,(const char*)str);
	} 
	return (u8*)strx;
}
//��AIR780��������
//cmd:���͵������ַ���(����Ҫ��ӻس���),��cmd<0XFF��ʱ��,��������(���緢��0X1A),���ڵ�ʱ�����ַ���.
//ack:�ڴ���Ӧ����,���Ϊ��,���ʾ����Ҫ�ȴ�Ӧ��
//waittime:�ȴ�ʱ��(��λ:10ms)
//����ֵ:0,���ͳɹ�(�õ����ڴ���Ӧ����)
//       1,����ʧ��
u8 AIR780_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART2_RX_STA=0;USART2_RX_REC_ATCOMMAD=1;
	if((u32)cmd<=0XFF)
	{
		while(DMA1_Channel7->CNDTR!=0);	//�ȴ�ͨ��7�������   
		USART2->DR=(u32)cmd;
	}else u2_printf("%s\r\n",cmd);//��������
	if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			delay_ms(10);
			if(USART2_RX_STA&0X8000)//���յ��ڴ���Ӧ����
			{
				if(AIR780_check_cmd(ack))break;//�õ���Ч���� 
				USART2_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	USART2_RX_STA=0;USART2_RX_REC_ATCOMMAD=0;
	return res;
} 
/*
u8 sim900a_work_test(void)
{
	if(AIR780_send_cmd((u8 *)"AT",(u8 *)"OK",100))
	{
		if(AIR780_send_cmd((u8 *)"AT",(u8 *)"OK",100))return SIM_COMMUNTION_ERR;	//ͨ�Ų���
	}
if(AIR780_send_cmd((u8 *)"AT",(u8 *)"OK",100))
	{
		if(AIR780_send_cmd((u8 *)"AT",(u8 *)"OK",100))return SIM_COMMUNTION_ERR;	//ͨ�Ų���
	}	
if(AIR780_send_cmd((u8 *)"AT",(u8 *)"OK",100))
	{
		if(AIR780_send_cmd((u8 *)"AT",(u8 *)"OK",100))return SIM_COMMUNTION_ERR;	//ͨ�Ų���
	}	
if(AIR780_send_cmd((u8 *)"AT",(u8 *)"OK",100))
	{
		if(AIR780_send_cmd((u8 *)"AT",(u8 *)"OK",100))return SIM_COMMUNTION_ERR;	//ͨ�Ų���
	}	
if(AIR780_send_cmd((u8 *)"AT",(u8 *)"OK",100))
	{
		if(AIR780_send_cmd((u8 *)"AT",(u8 *)"OK",100))return SIM_COMMUNTION_ERR;	//ͨ�Ų���
	}		
	if(AIR780_send_cmd((u8 *)"AT+CPIN?",(u8 *)"READY",400))return SIM_CPIN_ERR;	//û��SIM��
	if(AIR780_send_cmd((u8 *)"AT+CREG?",(u8 *)"0,1",400))
	{
		if(strstr((const char*)USART2_RX_BUF,"0,5")==NULL)
		{
			 if(!AIR780_send_cmd((u8 *)"AT+CSQ",(u8 *)"OK",200))	
			 {
					memcpy(SIM900_CSQ,USART2_RX_BUF+15,2);
			 }
			 return SIM_CREG_FAIL;	//�ȴ����ŵ�����
		}
	}
	return AIR_OK;
}*/
u8 AIR_Dect(void)//���ڵõ���Ӧ�Ľ��
{
	u8 res;
	res=AIR780_Init();	
	switch(res)
	{
		case AIR_OK:
			UART3SendString((u8*)"4Gģ���Լ�ɹ�\r\n",strlen("4Gģ���Լ�ɹ�\r\n"));
			break;
		case AIR_COMMUNTION_ERR:
			UART3SendString((u8*)"��4Gģ��δͨѶ�ɹ�����ȴ�\r\n",strlen("��4Gģ��δͨѶ�ɹ�����ȴ�\r\n"));
			break;
		case AIR_Init_ERR:
			UART3SendString((u8*)"��ʼ��������ȴ�\r\n",strlen("��ʼ��������ȴ�\r\n"));
			break;
		case AIR_NULL:
			UART3SendString((u8*)"û���źţ���ȴ�\r\n",strlen("û���źţ���ȴ�\r\n"));
			break;
		case AIR_CPIN_ERR:
			UART3SendString((u8*)"û�п�����ȴ�\r\n",strlen("û�п�����ȴ�\r\n"));
			break;
		case AIR_CMGF_ERR:
			UART3SendString((u8*)"ע�������С�����\r\n",strlen("ע�������С�����\r\n"));	
			break;		
		default:
			break;
	}
	return res;
}
u8 PUD_AIR_Dect(void)//PUDģʽ�����ڵõ���Ӧ�Ľ��
{
	u8 res;
	res=PUD_AIR780_Init();	
	switch(res)
	{
		case AIR_OK:
			UART3SendString((u8*)"4Gģ���Լ�ɹ�\r\n",strlen("4Gģ���Լ�ɹ�\r\n"));//��ӡ����������
			break;
		case AIR_COMMUNTION_ERR:
			UART3SendString((u8*)"��4Gģ��δͨѶ�ɹ�����ȴ�\r\n",strlen("��4Gģ��δͨѶ�ɹ�����ȴ�\r\n"));
			break;
		case AIR_Init_ERR:
			UART3SendString((u8*)"��ʼ��������ȴ�\r\n",strlen("��ʼ��������ȴ�\r\n"));
			break;
		case AIR_NULL:
			UART3SendString((u8*)"û���źţ���ȴ�\r\n",strlen("û���źţ���ȴ�\r\n"));
			break;
		case AIR_CPIN_ERR:
			UART3SendString((u8*)"û�п�����ȴ�\r\n",strlen("û�п�����ȴ�\r\n"));
			break;
		case AIR_CMGF_ERR:
			UART3SendString((u8*)"ע�������С�����\r\n",strlen("ע�������С�����\r\n"));	
			break;		
		default:
			break;
	}
	return res;
}


u8 AIR780_SendChMessageHex(u8*number,u8*content)
{
	u8 cmd[100];
	sprintf((char*)cmd,"AT+CMGS=\"%s\"",number);
	if(AIR780_send_cmd(cmd,(u8 *)">",100))	return AIR_CMGS_ERR;
	sendCommandHex((char*)content,strlen((char*)content),1);
	if(AIR780_send_cmd((u8 *)0x1A,(u8 *)"+CMGS:",1500))	return AIR_CMGS_SEND_FAIL;
	return AIR_OK;
}
u8 AIR780_SendChMessage(u8*number,u8*content)
{
	u8 cmd[100];
	sprintf((char*)cmd,"AT+CMGS=\"%s\"",number);
	if(AIR780_send_cmd(cmd,(u8 *)">",100))	return AIR_CMGS_ERR;
	UART2SendString(content,strlen((char*)content));
	if(AIR780_send_cmd((u8 *)0x1A,(u8 *)"+CMGS:",1500))	return AIR_CMGS_SEND_FAIL;
	return AIR_OK;
}
u8 PUD_AIR780_SendChMessage(u8*content)
{
	AIR780_send_cmd((u8 *)"AT+CMGF=0",(u8 *)"OK",100);//����ΪPUDģʽ
//	RES=0;
	if(AIR780_send_cmd((u8 *)"AT+CMGS=18",(u8 *)">",100)==1) return 0;
	UART2SendString(content,strlen((char*)content));
	UART2SendString((u8*)"11000D91",strlen((char*)"11000D91"));
	UART2SendString((u8*)phone_NumOK,strlen((char*)phone_NumOK));
	UART2SendString((u8*)"000800",strlen((char*)"000800"));
	switch(MSGBZ)
	{
		case 1:UART2SendString((u8*)MSG1,strlen((char*)MSG1));break;
		case 2:UART2SendString((u8*)MSG2,strlen((char*)MSG2));break;
		case 3:UART2SendString((u8*)MSG3,strlen((char*)MSG3));break;
		case 4:UART2SendString((u8*)MSG4,strlen((char*)MSG4));break;
		default:UART2SendString((u8*)MSG,strlen((char*)MSG));
	}
	UART2SendString((u8*)QJMUnicode,strlen((char*)QJMUnicode));
	if(AIR780_send_cmd((u8 *)0x1A,(u8 *)"+CMGS:",2000)==1) return 0;
	return 1;
}
u8 AIR780_Init(void)//PUDģʽ�������
{
	u8 n;
	for(n=0;n<10;n++)
	{
		if(AIR780_send_cmd((u8 *)"AT",(u8 *)"OK",100))
		{
			if(AIR780_send_cmd((u8 *)"AT",(u8 *)"OK",100))return AIR_COMMUNTION_ERR;	//ͨ�Ų���
		}
	}
	delay_ms(10);
	for(n=0;n<10;n++)
	{
		if(AIR780_send_cmd((u8 *)"AT&F",(u8 *)"OK",100))
		{
			if(AIR780_send_cmd((u8 *)"AT&F",(u8 *)"OK",100))return AIR_Init_ERR;	//��ʼ������
		}
	}
	delay_ms(10);
	for(n=0;n<10;n++)
	{
		
		AIR780_send_cmd((u8 *)"AT+CSQ",(u8 *)"OK",100);//return AIR_NULL;	//û���ź�
		
	}
	delay_ms(10);
	for(n=0;n<10;n++)
	{
		
		if(AIR780_send_cmd((u8 *)"AT+CPIN?",(u8 *)"READY",400))return AIR_CPIN_ERR;	//û��SIM��
		
	}
	delay_ms(10);
	for(n=0;n<10;n++)
	{
		
		if(AIR780_send_cmd((u8 *)"AT+CREG?",(u8 *)"0,1",400))return AIR_CREG_ERR;	//CREG
		
	}
	delay_ms(10);
	for(n=0;n<10;n++)
	{
		
		AIR780_send_cmd((u8 *)"AT+CREG?",(u8 *)"0,1",400);
		
	}
	delay_ms(10);
	for(n=0;n<10;n++)
	{
		
		if(AIR780_send_cmd((u8 *)"AT+CMGF=1",(u8 *)"OK",100))	return AIR_CMGF_ERR;
		
	}
	delay_ms(10);
	for(n=0;n<10;n++)
	{
		
		if(AIR780_send_cmd((u8 *)"AT+CSCS?",(u8 *)"OK",100))	return AIR_CMGF_ERR;
		
	}
	delay_ms(10);
	for(n=0;n<10;n++)
	{
		
		if(AIR780_send_cmd((u8 *)"AT+CSMP=17,167,0,8",(u8 *)"OK",100))	return AIR_CSMP_ERR;
		
	}
	delay_ms(10);
	return AIR_OK;
}
u8 PUD_AIR780_Init(void)//PUDģʽʹ��
{
	u8 n;
	for(n=0;n<10;n++)
	{
		if(AIR780_send_cmd((u8 *)"AT",(u8 *)"OK",100))
		{
			if(AIR780_send_cmd((u8 *)"AT",(u8 *)"OK",100))return AIR_COMMUNTION_ERR;	//ͨ�Ų���
		}
	}
	delay_ms(10);
	for(n=0;n<10;n++)
	{
		if(AIR780_send_cmd((u8 *)"AT&F",(u8 *)"OK",100))
		{
			if(AIR780_send_cmd((u8 *)"AT&F",(u8 *)"OK",100))return AIR_Init_ERR;	//��ʼ������
		}
	
	}
	delay_ms(10);
	for(n=0;n<5;n++)
	{
		AIR780_send_cmd((u8 *)"AT+CSQ",(u8 *)"OK",100);//�鿴�ź�
	}
	delay_ms(10);
	for(n=0;n<5;n++)
	{
		if(AIR780_send_cmd((u8 *)"AT+CPIN?",(u8 *)"READY",400))//SIM����û��
		{
			return AIR_CPIN_ERR;	//û��SIM��
		}
	}
	delay_ms(10);
	for(n=0;n<5;n++)
	{
		if(AIR780_send_cmd((u8 *)"AT+CREG?",(u8 *)"0,1",400))return AIR_CREG_ERR;	//CREG
	}
	delay_ms(10);
	for(n=0;n<5;n++)
	{
		
		AIR780_send_cmd((u8 *)"AT+CREG?",(u8 *)"0,1",400);
		
	}
	delay_ms(10);
	for(n=0;n<5;n++)
	{
		
		if(AIR780_send_cmd((u8 *)"AT+CMGF=0",(u8 *)"OK",100))	return AIR_CMGF_ERR;//ԭ����1//ѡ��PUD��ʽ
		
	}
	delay_ms(10);
	for(n=0;n<5;n++)
	{
		
		AIR780_send_cmd((u8 *)"AT+CSCS?",(u8 *)"OK",100);//��ѯ�ַ���Χ
		
	}
	delay_ms(10);
	for(n=0;n<5;n++)
	{
		
		if(AIR780_send_cmd((u8 *)"AT+CSMP=17,167,0,8",(u8 *)"OK",100))	return AIR_CSMP_ERR;//���ö��Ų���
		
	}
	delay_ms(10);
	return AIR_OK;
}

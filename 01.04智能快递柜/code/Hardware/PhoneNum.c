#include "stm32f10x.h"                  // Device header
#include "string.h"
#include <stdio.h>
#include <usart2.h>
//extern char phone_center[100];//短信中心号码最后为总发送的内容
extern char phone_Num[100];//号码处理之前
extern char phone_NumOK[100];
extern char MSG[100];//短信
extern char MSG1[100];//短信
extern char MSG2[100];//短信
extern char MSG3[100];//短信
extern char MSG4[100];//短信
extern char phone_center[100];
char* PhoneCENNum;
void swapCharactersInString(char* str) //颠倒
{
    int length = strlen(str);
    for (int i = 0; i < length - 1; i += 2) {
        char temp = str[i];
        str[i] = str[i + 1];
        str[i + 1] = temp;
    }
}
void PhoneNum(char* PhoneNum, char* PhoneNumOK)//19854252590
{
	sprintf(PhoneNumOK,"%d",86);//PhoneNumOK="86"
	char str[10]="F";
	strcat(PhoneNumOK,PhoneNum);//PhoneNumOK="8619854252590"
	strcat(PhoneNumOK,str);//PhoneNumOK="8619854252590F"
	swapCharactersInString(PhoneNumOK); //PhoneNumOK="689158242595F0"
}
void PhoneNumEND(void)
{
//	PhoneNum(phone_Num, phone_NumOK);
//	strcat(phone_center,"11000D91");
//	strcat(phone_center,phone_NumOK);
//	strcat(phone_center,"000800");
//	strcat(phone_center,MSG);
//	UART3SendString((u8*)phone_center,strlen((char*)phone_center));
}
//void PhoneNumEND1(char* PhoneCENNum)
//{
//	PhoneNum(phone_Num, phone_NumOK);;	
//	strcat(PhoneCENNum,"11000D91");
//	strcat(PhoneCENNum,phone_NumOK);
//	strcat(PhoneCENNum,"000800");
//	strcat(PhoneCENNum,MSG1);
//}
//void PhoneNumEND2(char* PhoneCENNum)
//{
//	PhoneNum(phone_Num, phone_NumOK);;	
//	strcat(PhoneCENNum,"11000D91");
//	strcat(PhoneCENNum,phone_NumOK);
//	strcat(PhoneCENNum,"000800");
//	strcat(PhoneCENNum,MSG1);
//}
//void PhoneNumEND3(char* PhoneCENNum)
//{
//	PhoneNum(phone_Num, phone_NumOK);;	
//	strcat(PhoneCENNum,"11000D91");
//	strcat(PhoneCENNum,phone_NumOK);
//	strcat(PhoneCENNum,"000800");
//	strcat(PhoneCENNum,MSG1);
//}
//void PhoneNumEND4(char* PhoneCENNum)
//{
//	PhoneNum(phone_Num, phone_NumOK);;	
//	strcat(PhoneCENNum,"11000D91");
//	strcat(PhoneCENNum,phone_NumOK);
//	strcat(PhoneCENNum,"000800");
//	strcat(PhoneCENNum,MSG1);
//}

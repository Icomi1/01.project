#ifndef __AIR780_H
#define __AIR780_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
#define AIR_OK 0
#define AIR_COMMUNTION_ERR 0xff
#define AIR_Init_ERR 0xfe
#define AIR_NULL 0xfd
#define AIR_CPIN_ERR 0Xfc
#define AIR_CMGF_ERR     0xfb

#define AIR_CMGF_ERR 0xfa
#define AIR_CREG_ERR 0xf9
#define AIR_CSCA_ERR 0xf8
#define AIR_CSMP_ERR 0Xf7
#define AIR_CMGS_ERR       0xf6
#define AIR_CMGS_SEND_FAIL       0xf5

u8 AIR780_Init(void);
u8 PUD_AIR780_Init(void);
//void errorLog(int num);
//void Sys_Soft_Reset(void);
//void sendMessage(char *number,char *msg);
//unsigned int sendCommand(char *Command, char *Response, unsigned long Timeout, unsigned char Retry);
void USART2_CLR_Buf(void);
unsigned char nibble2c(unsigned char c);
unsigned char hex2c(unsigned char c1, unsigned char c2);
unsigned int sendCommandHex(char *Command, unsigned long Timeout, unsigned char Retry);
void AIR_at_response(u8 mode);
u8* AIR780_check_cmd(u8 *str);
u8 AIR780_send_cmd(u8 *cmd,u8 *ack,u16 waittime);
u8 AIR_Dect(void);
u8 PUD_AIR_Dect(void);
u8 AIR780_SendChMessage(u8*number,u8*content);
u8 AIR780_SendChMessageHex(u8*number,u8*content);
u8 PUD_AIR780_SendChMessage(u8*content);
#endif

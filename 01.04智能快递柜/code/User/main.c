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

/*PUD模式参数*/
//短信中心号码最后为总发送的内容
char phone_Num[100];//号码处理之前，原始号码
char phone_NumOK[100];
char MSG[100]= "07915185461457F521000D91685185461457F50008044F60597D";//短信
char MSG1[100]= "07915185461457F521000D91685185461457F50008044F60597D";//1号快递柜短信
char MSG2[100]= "07915185461457F521000D91685185461457F50008044F60597D";//2号快递柜短信
char MSG3[100]= "07915185461457F521000D91685185461457F50008044F60597D";//3号快递柜短信
char MSG4[100]= "07915185461457F521000D91685185461457F50008044F60597D";//4号快递柜短信
char QJM[10];//
char QJMUnicode[10];//
char XH[10];//
char XHUnicode[10];//
/*时钟模块参数*/
uint8_t D_Buf[11]={0};  //存储年月日
uint8_t Time_Buf[10]={0}; //存储时分秒
uint8_t year=20;           //年高两位
uint8_t week[2]={0};               //星期
uint8_t KEY0_Num=0;           //切换调整项
//////////////////////////////////////////////////////////
//uint64_t sjh=1000;
int RES,RETURN;
char qhmUnicode[100];
char qhm[100];
uint64_t QuJianMa1,QuJianMa2,QuJianMa3,QuJianMa4;

int KeyNum=0;//键值
unsigned char  func_index =0;//函数标志，值就是执行的函数所在元素
unsigned char last_index =10;//执行完操作之后的标志

typedef struct//定义结构体key_table
{
	unsigned char current;//现在所在页面层数	
	unsigned char down;//向下翻
	unsigned char enter;//进入功能，确认
	void (*current_operation)();//执行函数。显示图片
}key_table;
//创建一个包含 9 个 key_table 结构体元素的数组 table
key_table table[9]=
{
//首页
	{0,0,1,(*my_testA0)},//年月日主页

	{1,2,3,(*my_testB1)},//智能快递柜控制系统页，箭头在上：取件人取件
	{2,1,4,(*my_testB2)},//智能快递柜控制系统页，箭头在下：快递员存件

	{3,3,3,(*my_testC3)},//请输入取件码
	{4,4,5,(*my_testC4)},//快递员验证身份

	{5,6,7,(*my_testD5)},//选择快递柜类型，箭头在上：大柜
	{6,5,8,(*my_testD6)},//选择快递柜类型，箭头在下：小柜
	{7,7,5,(*my_testE7)},//分配柜号：1号柜和2号柜，大柜
	{8,8,5,(*my_testE8)},//分配柜号：3号柜和4号柜，小柜
};
void (*current_operation_index)();//储存当前要执行的函数
int main()
{
	LED_Init();//LED初始化
//Ds1302Init();//在第一次设置好时间之后下载，然后之后注释掉
	NVIC_Configuration(); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	OLED_Init();//oled显示屏初始化
	OLED_ShowChinese(16,16,"系统初始化中");
	OLED_Update();
	KEY_4x4_Init();//矩阵按键初始化
	OLED_Clear();//清屏
	Servo_Init();//舵机初始化
//	Key_Init();//按键初始化
//	TIM3_Int_Init();
	MyUSART_Init();//串口1语音模块初始化，波特率固定为9600
	USART2_Init(115200);	//串口2 4G通信模块
	USART3_Init(115200);	//串口3 PC端
	RETURN=PUD_AIR_Dect();//4G模块初始化+信息提示
	switch(RETURN)//用switch语句判断4G模块是否自检成功，用oled显示
	{
		case AIR_OK:
		{
			OLED_ShowString(8,16,"4G",8);
			OLED_ShowChinese(24,16,"模块自检成功");
			OLED_Update();
		}
		break;
		default:
		{
			OLED_ShowString(8,16,"4G",8);
			OLED_ShowChinese(24,16,"模块自检失败");
			OLED_Update();
		}
	}
	delay_ms(1000);
	OLED_ShowChinese(8,16,"系统初始化完成");
	OLED_Update();
	UART3SendString((u8*)"初始化完成\r\n",strlen((char*)"初始化完成\r\n"));//给电脑发送 完成初始化 的信息
	delay_ms(1000);
	OLED_Clear();
	OLED_ShowImage(32,0,70,70,KuaiDi);//图标
	OLED_Update();
	Timer_Init();//定时器初始化
	Usart_SendString(USART1,"ASR_ON");//给语音模块发送“ASR_ON”
	while(1)
	{
		KEY_Scan(&KeyNum);//按键扫描函数，将扫描的值写入KeyNum中
		if(KeyNum)
		{
			if(KeyNum==14)//向下翻
         {
            func_index = table[func_index].down;    //
				KeyNum=0;
//            while(!KeyNum);//跳出循环
         }
			if(KeyNum==11)//进入/确认
         {
             func_index = table[func_index].enter;    //
			 	KeyNum=0;
//            while(!KeyNum);
			}
			if(KeyNum==16)//返回
			{
				func_index=0;
			}
			if (func_index != last_index)//判断标志是否改变，在操作之后，切页面
			{
				//存储当前要执行的函数//table[0].current_operation=my_testA0=current_operation_index
				current_operation_index = table[func_index].current_operation;//table[0].current_operation=*my_testA0
				if(((last_index-func_index)>=2)||((func_index-last_index)>=2)||last_index==0)//判断是否回到主界面
				{
					OLED_Clear();
				}
				(*current_operation_index)();//执行函数
//				UART3SendString((u8*)"到这\r\n",strlen((char*)"到这\r\n"));//
				last_index = func_index;//
			} 	
	
		}
			
		
	}

}

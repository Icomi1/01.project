/**********************************************************************
                 Maxwell GuangZhou   2023.06-2023.07

       RFID



***********************************************************************/
#include "Project.h"

char recv_buf_rfid[128];//为了接收RFID模块的响应信息
char rt_buf[10];//存放接收数据
int uart1_fd;
char request_buff[] = {0x07,0x02,0x41,0x01,0x52,0x00,0x03};   //请求指令的数据帧 下标是5的是BCC校验和：
int RFID_read_sucess_flag = 0;  //ID卡是否读取成功的标志位  没读到置0 读到是1


int err_rfid = 0;


//有了请求指令和防碰撞指令就可以从串口中读取数据

void rfid_start()
{

	UART1_Init();
	while(1)
  {
	RFID_read_sucess_flag = UART1_Init2();


		if(RFID_read_sucess_flag == 1)
		{

			sleep(2);
			read_JPEG_file("success.jpg",0,0);
			sleep(2);
			desk = 1;
			teacher_ways_flag = 0;  // == 1 就是老师
			student_ways_flag = 0;  // == 1 就是学生
			stu_atten_flag = 0;     //学生打卡方式   1蓝牙 2刷卡  3人脸  4密码
			tea_atten_flag = 0;     //老师打卡方式   1蓝牙 2刷卡  3人脸  4密码
			again_re_select_flag = 0;//失败了显示
			// state_machine = 0;
			RFID_read_sucess_flag = 0;
			break;
		}

	//    if(RFID_read_sucess_flag == 2)
	// 	{
	// 		 printf("fail to read the card!\n");

	// 		 read_JPEG_file("error.jpg",0,0);
	// 		 sleep(2);
	// 		 err_rfid++;
	// 		 if (err_rfid == 3)
	// 		 {
	// 		  printf("三次刷卡错误，回到桌面\n");
	// 		  err_rfid = 0;

	// 		  break;
	// 		 }
	// 	}


  }



}




 void UART1_Init()			//已经在ctrl中初始化过了
 {
	//1.打开UART
    uart1_fd = open("/dev/ttySAC1", O_RDWR|O_NOCTTY);
    if (uart1_fd == -1)
    {
        printf("uart文件打开失败\n");
        // return -1;
    }




	//1.定义两个串口结构体
	struct termios  old_attr; //保存串口的旧属性
	struct termios  new_attr; //保存串口的新属性

	//2.保存串口的原本的属性
	tcgetattr (uart1_fd,&old_attr);

	//3.配置数据位  8bit
	new_attr.c_cflag &= ~CSIZE; //清空数据位
	new_attr.c_cflag |=  CS8;   //8位数据位

	//4.配置停止位  1bit
	new_attr.c_cflag &= ~CSTOPB; //1位数据位

	//5.配置校验位  none
	new_attr.c_cflag &= ~PARENB; //不使用校验位

	//6.配置波特率  9600bps
	cfsetospeed (&new_attr,B9600);
	cfsetispeed (&new_attr,B9600);

	//7.配置本地连接和接收使能
	new_attr.c_cflag |= CREAD | CLOCAL;

	//8.设置等待时间和最少接收字符
	new_attr.c_cc[VTIME] = 0;
	new_attr.c_cc[VMIN]  = 1;

	//9.设置为原始模式（指的是所有输入的字符都会被当做普通字符）
	cfmakeraw (&new_attr);

	//10.确保参数生效 （立即）
	tcsetattr(uart1_fd, TCSANOW,&new_attr);

	//11.清空输入输出缓冲区
	tcflush (uart1_fd,TCIOFLUSH);


}



int UART1_Init2()
{

    //给RFID阅读器发送请求指令并处理响应信息
    RFID_GetBCC(request_buff);//计算校验和
	int err_rfid = 0;
    for(;;)
    {
        //刷新串口缓冲区
        tcflush (uart1_fd,TCIOFLUSH);
        // 发送指令给RFID模块
        write (uart1_fd,request_buff,sizeof(request_buff));
        usleep(50*1000);
        //  读卡,读卡的响应信息
        read(uart1_fd,recv_buf_rfid,sizeof(recv_buf_rfid));

        //判断响应信息是否成功
        if(recv_buf_rfid[2] == 0x00)
        {
            printf("sucess read the card!\n");
            bzero(recv_buf_rfid,sizeof(recv_buf_rfid));		//清空数组
            RFID_read_sucess_flag =  PiccAnticoll(uart1_fd);
             break;
        }


    }

	return RFID_read_sucess_flag;

}



//防碰撞
int PiccAnticoll(int fd)
{
	int ret;
	unsigned char tx_buf[8];//存放要发送的数据
	int card_ID=0;
	tx_buf[0]=0x08;//命令长度
	tx_buf[1]=0x02;//命令类型
	tx_buf[2]='B';//命令
	tx_buf[3]=0x02;//数据长度
	tx_buf[4]=0x93;//碰撞级别选择1级
	tx_buf[5]=0x00;
	tx_buf[6]=CalBCC(tx_buf,tx_buf[0]-2);//校验
	tx_buf[7]=0x03;//结束

	write(uart1_fd,tx_buf,8);//发送数据给从机
	usleep(50000);//延时
	ret=read(uart1_fd,rt_buf,10);//接收从机应答消息
	if(ret<0)//接收返回值小于0接收失败
	{
		printf("read err\n");
		return 0;
	}
	if(ret==10)//接收到长度10的数据
	{
		if(rt_buf[2]==0x00)//判断status位为0表示成功
		{
			card_ID=(rt_buf[7]<<24)|(rt_buf[6]<<16)|(rt_buf[5]<<8) |rt_buf[4];//读取buf中数据
			printf("ID=%x\n",card_ID);
			return 1;
		}
	}
	return 2;
}






//计算BCC SerBfr是0x07  BCC 前面的数异或取反
//计算数据帧的校验和
void RFID_GetBCC(char *buf)
{
	int i;
	char BCC = 0;

	for(i=0; i<(buf[0]-2); i++)
	{
		BCC ^= buf[i];
	}
	buf[buf[0]-2] = ~BCC;
}



//校验
unsigned char CalBCC(unsigned char *buf,int n)
{
	int i;
	unsigned char bcc=0;
	for(i=0;i<n;i++)
	{
		bcc^=*(buf+i);
	}
	return (~bcc);
}
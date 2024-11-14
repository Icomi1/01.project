/**********************************************************************
                 QiJf   2023.08-2023.09

       Blueteeth


***********************************************************************/
#include "Project.h"


int uart2_fd; //存储UART2外设的文件代号

char cmd_test[] = "AT\r\n"; //测试指令
char cmd_change_name[] = "AT+NAMEduoyu\r\n";//更改模块名字为kiang
char cmd_reset[] = "AT+RESET\r\n";//软重启
char cmd_set_slave_mode[] = "AT+ROLE<0>\r\n";//设置成从模式
char cmd_find_mode[] = "AT+ROLE\r\n";//查询主模式？从模式？
char recv_buf_blue[128] ; //为了接收蓝牙模块的响应信息




void blue_start()
{
		for(;;)
	{

		/******************兵**************************/
		// 1.打开开发板的UART2 防止该串口影响基本串口
		uart2_fd = open("/dev/ttySAC2", O_RDWR|O_NOCTTY);
		if (uart2_fd == -1) { printf("uart2文件打开失败\n"); }
		UART2_Init();                //蓝牙的的串口的初始化
		// 2.配置UART2的参数  字符格式 + 通信速率 9600bps


		// 3.响应测试
		tcflush (uart2_fd,TCIOFLUSH); //刷新串口的缓冲区
		write(uart2_fd,cmd_test,strlen(cmd_test)); //发送指令给蓝牙模块
		read(uart2_fd,recv_buf_blue,sizeof(recv_buf_blue)); // 读取蓝牙模块的响应信息
		//	usleep(50*1000);
		printf("rec = %s\n",recv_buf_blue);
		// bzero(recv_buf_blue,sizeof(recv_buf_blue));

		// 4.查询主模式？从模式？
		tcflush (uart2_fd,TCIOFLUSH); //刷新串口的缓冲区
		write(uart2_fd,cmd_find_mode,strlen(cmd_find_mode));
		read(uart2_fd,recv_buf_blue,sizeof(recv_buf_blue));
		printf("rec = %s (0 —> slave dev)\n\n",recv_buf_blue);
			if (0==(strncmp(recv_buf_blue,"desk",4)))
		{
			printf("SUCCESS   收到的指令是  %s\n",recv_buf_blue);
			read_JPEG_file("success.jpg",0,0);
			break;
		}
		else if (0!=(strncmp(recv_buf_blue,"desk",4)))
		{
			printf("FAILED   收到的指令是  %s\n",recv_buf_blue);
			read_JPEG_file("error.jpg",0,0);
			break;

		}
		bzero(recv_buf_blue,sizeof(recv_buf_blue));

		// 5.修改蓝牙模块的名字
		tcflush (uart2_fd,TCIOFLUSH); //刷新串口的缓冲区
		write(uart2_fd,cmd_change_name,strlen(cmd_change_name));
		read(uart2_fd,recv_buf_blue,sizeof(recv_buf_blue));
		usleep(50*1000);
		printf("rec = %s\n",recv_buf_blue);
		bzero(recv_buf_blue,sizeof(recv_buf_blue));




		// 6.重启生效
		tcflush (uart2_fd,TCIOFLUSH); //刷新串口的缓冲区
		write(uart2_fd,cmd_reset,strlen(cmd_reset));
	/*************strcmp(*****兵**************************/

	}


    // sleep(2);

    sleep(2);
    desk = 1;
    teacher_ways_flag = 0;  // == 1 就是老师
    student_ways_flag = 0;  // == 1 就是学生
    stu_atten_flag = 0;     //学生打卡方式   1蓝牙 2刷卡  3人脸  4密码
    tea_atten_flag = 0;     //老师打卡方式   1蓝牙 2刷卡  3人脸  4密码
    again_re_select_flag = 0;//失败了显示
    state_machine = 0;



}




void  UART2_Init()//串口2的初始化
{
	//1.定义两个串口结构体
	struct termios  old_attr; //保存串口的旧属性
	struct termios  new_attr; //保存串口的新属性

	//2.保存串口的原本的属性
	tcgetattr (uart2_fd,&old_attr);

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
	tcsetattr(uart2_fd, TCSANOW,&new_attr);

	//11.清空输入输出缓冲区
	tcflush (uart2_fd,TCIOFLUSH);
}

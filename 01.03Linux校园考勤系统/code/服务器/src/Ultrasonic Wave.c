/**********************************************************************
                 QiJf   2023.08-2023.09
*				  超声波引脚接线说明
**		  VCC-----接5V电源-----J10的02号引脚
*    	  TRIG----接GPIOC7-----J10的15号引脚
*         ECHO----接GPIOC8-----J10的17号引脚
*         GND-----接地---------J10的19号引脚
** 超声波传感器的测量周期是60ms以上，接线的时候先接地
 ***********************************************************************/
  #include "Project.h"

struct timespec ts ={0,50000};// delay for 1s


 int ditance = 0;

void *wave(void * sdd)
{
	// int ditance = 0; //用于存储超声波传感器计算的距离

	//1.打开设备文件
	int fd = open("/dev/sr04_dev",O_RDWR);
	if(fd < 0)
	{perror("Open HC-SR04 Device Error");}		// return -1;	}


	int wave_time = 0;
	//2.每隔1s采集一次超声波距离并输出到终端
	while(1)
	{
				//从超声波的驱动中读取计算好的距离

		read(fd,&ditance,4);
		//输出对应的结果，单位是mm
		printf(" 您与客户端的距离是 %d mm\n",ditance);

		//用于设置超声波的测量周期，必须60ms以上，防止信号干扰
		  sleep(1);
		  wave_time ++;
		  if(wave_time == 5)
		  break;
	    // pthread_delay_np(&sleep_interval);
		// nanosleep(&ts, NULL);
	}

	// char server_buf[]={0};

	// //创建服务器的套接字
	// int sock_server_fd = socket(AF_INET,SOCK_STREAM,0);
    // send(sock_fd,send_buf,strlen(send_buf),0);
    // printf("  请<点击屏幕>或<语音控制>选择考勤方式");


	// return 0;
}


void wave2()
{

//1.打开设备文件
	int wave_fd = open("/dev/sr04_dev",O_RDWR);
	if(wave_fd < 0)
	{perror("Open HC-SR04 Device Error");}		// return -1;	}


	int wave_time = 0;
	//2.每隔1s采集一次超声波距离并输出到终端
	while(1)
	{
		label1:
		read(wave_fd,&ditance,4);
		//输出对应的结果，单位是mm
		printf(" Distance between you and machine is %d mm\n",ditance);

		//用于设置超声波的测量周期，必须60ms以上，防止信号干扰
		  sleep(1);
		  wave_time ++;

		  if(wave_time == 5)

		/*开始给ubuntu发文字：“欢迎使用考勤系统”*/
		// {
		// 		//1.创建客户端的套接字
		// 	int sock_fd = socket(AF_INET,SOCK_STREAM,0);
		// 	if(sock_fd == -1)
		// 	{
		// 		perror("create socket error");
		// 		return -1;
		// 	}

		// 	//2.客户端可以连接服务器  大家需要根据自己的IP地址进行修改
		// 	struct sockaddr_in	server_addr;
		// 	bzero(&server_addr,sizeof(server_addr));//清空结构体

		// 	server_addr.sin_family = AF_INET;						//协议族 Ipv4
		// 	server_addr.sin_port   = htons(6666);					//端口号  需要转换为网络字节序
		// 	server_addr.sin_addr.s_addr = inet_addr("192.168.14.43");	//服务器IP地址  需要转换为网络字节序


		// 	connect(sock_fd,(struct sockaddr *)&server_addr,sizeof(server_addr));

		// 	//3.数据收发
		// 	// char send_buf[40] = {0};
		// 	send(sock_fd,"welcome to school",strlen(welcome to school),0);

		// }
		{
			close(wave_fd);
		  break;
		}
		else goto label1;
	    // pthread_delay_np(&sleep_interval);
		// nanosleep(&ts, NULL);
	}

	printf("welcome to school   now come into attendence system\n");




}
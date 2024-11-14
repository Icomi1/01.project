/*******************************************************************************
*
*
*	这是一个关于TCP协议传输数据的服务器程序，目的是接收客户端的连接，并把客户端
*	发送的数据输出到终端
*
* 	注意：使用TCP协议的过程中，服务器的程序流程如下
*
*
*  	1.创建套接字	socket
*  	2.绑定端口号	bind
*  	3.监听客户端	listen
*  	4.接受客户端    accept
*  	5.数据的收发	recv & send
*  	6.关闭服务器	close
*
* 	注意：服务器的端口号必须要和客户端使用的一致，否则无法顺利接受数据，另外运
* 	行程序的时候需要确保服务器和客户端处于同一个网络
*
* *****************************************************************************/
 #include "Project.h"


void *server_start(void* arg)
{
	//1.创建socket套接字   IPV4    TCP协议
	int sock_fd = socket(AF_INET,SOCK_STREAM,0);
	if(sock_fd == -1)
	{
		perror("create socket error");

	}


	//2.套接字socket绑定端口，注意需要把参数转换为网络字节序
	struct sockaddr_in	my_addr;
	bzero(&my_addr,sizeof(my_addr));//清空结构体

	my_addr.sin_family = AF_INET;				//协议族 Ipv4
	my_addr.sin_port   = htons(6666);			//端口号  需要转换为网络字节序
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);//IP地址  需要转换为网络字节序  可以接受任何连接

	bind(sock_fd,(struct sockaddr *)&my_addr,sizeof(my_addr)); //把服务器的套接字和设置的参数绑定在一起


	//3.服务器开始侦听，监听的客户端的数量可以自行修改  目前为5
	listen(sock_fd,5);


	//4.接受客户端的连接，服务器在接收客户端连接的时候是可以顺便把客户端的信息(IP地址)保存下来
	struct sockaddr_in	client_addr;
	bzero(&client_addr,sizeof(client_addr));		//清空结构体

	unsigned int client_len = sizeof(client_addr);	//计算结构体大小


	int new_fd = accept(sock_fd,(struct sockaddr *)&client_addr,&client_len); //注意：必须确保接收accept函数的返回值


	//5.服务器和客户端进行数据的收发
	char recv_buf[50] = {0};
	while(1)
	{
		while(1)
		{
			bzero(recv_buf,sizeof(recv_buf));//清空数组
			recv(new_fd,recv_buf,sizeof(recv_buf),0);//接受数据 该函数相当于read 是会阻塞的
			//  printf("接收到的数据是 %s\n",recv_buf);



			//if (strstr(recv_buf2, "123") != NULL)
			if(strstr(recv_buf, "udent") != NULL)
			{
				printf("recieved : %s\n",recv_buf);
				read_JPEG_file("student_ways.jpg",0,0);
				desk = 0;               //不在桌面了
				// teacher_ways_flag = 0;  // == 1 就是老师
				student_ways_flag = 1;  // == 1 就是学生

				// again_re_select_flag = 0;//失败了显示
				// for(int i = 0;i<1000;i++)
				// {
					break;
				// }

			}


			// }
			// if(0 ==(strncmp(recv_buf,"Student use card",16)))
			// {
			// 	stu_atten_flag = 2;
			if(strstr(recv_buf, "acher") != NULL)
			{
				printf("接收到的数据是 %s\n",recv_buf);
				read_JPEG_file("teacher_ways.jpg",0,0);
				desk = 0;               //不在桌面了
				teacher_ways_flag = 1;  // == 1 就是老师

					break;

			}
			 //strstr(str1,str2) 函数用于判断字符串str2是否是str1的子串
			// if( strstr(recv_buf, "use card") != NULL)
			// {
			// 	tea_atten_flag = 2;
			//      break;
			// }


		}
		// pthread_exit(NULL);

	}

	//6.关闭套接字

	// return 0;
}
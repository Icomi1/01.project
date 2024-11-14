/**********************************************************************
                 Maxwell GuangZhou   2023.06-2023.07 

      *这个文件只有include头文件、宏定义声明
  
      *不要全局变量满天飞

***********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <strings.h>
#include "jpeglib.h" 
#include <string.h>
#include <pthread.h>  //多线程编程需要包含的
#include <termios.h>//头文件 配置串口参数必须包含
#include <sys/mman.h>
#include <linux/input.h>        /*输入子系统头文件*/
#include <arpa/inet.h>          /* TCP   */         
#include <sys/socket.h>         /* TCP   */      
#include <sys/time.h>
#include <time.h>
#include <assert.h>
#include <getopt.h>             /* getopt_long() */
#include <fcntl.h>              /* low-level i/o */
#include <errno.h>
#include <linux/videodev2.h>
#include <asm/types.h>          /* for videodev2.h */
#include <sys/ioctl.h>
 

/*******************************线程函数**********************************/
void *judge_location(void * arg);                        //判断坐标功能 
void *get_xy(void * arg);                                // 得到触摸坐标
void *server_thread(void *argv);                         //服务器线程
void *mainloop (void* argv);                             //处理采集画面的
void *state_judge(void* arg) ;                           //根据flag判断状态
void *state_ctrl(void* arg);                             //Proj_ctrl.c里面的
void *server_start(void* arg);
void *desk_gif(void * arg);
/*******************************LCD、触摸屏、JPEG*************************/
int lcd_init();
int read_JPEG_file (char * filename,int start_x,int start_y);
/*******************************RFID*************************************/
void UART1_Init();//RFID的线程
int UART1_Init2();
void RFID_GetBCC(char *buf);
int PiccAnticoll(int uart1_fd);
unsigned char CalBCC(unsigned char *buf,int n);
void rfid_start();
/*******************************Blueteeth********************************/
void blue_start();
void UART2_Init();//串口2的初始化

/*******************************Password*********************************/
void password_start();
int passwork_lock();
void keyshow_pass();

/*******************************CAMERA*********************************/
void init_device(void); //初始化摄像头模块
void init_mmap  (void); //对摄像头参数进行设置
void start_capturing(void);//开启视频采集
int  yuyv_to_rgb_pix(int Y,int Cb,int Cr);//转换一个像素点
int  yuyv_to_rgb(char * yuyvbuf);//转换一帧ARGB数据（YUYV转RGB32）
int  camera_to_lcd(int start_x,int start_y);//把转换后的数据写到LCD
void camera_start();//简化main函数  所以直接摄像头初始化函数
int  main_camera( );

#define CLEAR(x) memset (&(x), 0, sizeof (x))
#define  WIDTH  320  //采集的图像的宽度
#define  HEIGHT 240  //采集的图像的高度


int keyshow(); //输入时的图片切换
int  camera_lock(void );

/*****************************语音识别+服务器端*******************************/






/*****************************超声波*********************************/
 
void *wave(void * sdd);
void wave2();
/*******************************其它*********************************/
void open_machine_carton();
int  ControlMianUI(); //控制模块


/*************************声明全局变量状态机标志位****************************************/
extern   int desk;                //是否在桌面 0否 1在
extern   int teacher_ways_flag;   // == 1 就是老师
extern   int student_ways_flag;   // == 1 就是学生
extern   int stu_atten_flag;      //学生打卡方式   1蓝牙 2刷卡  3人脸  4密码
extern   int tea_atten_flag;      //老师打卡方式   1蓝牙 2刷卡  3人脸  4密码    
extern   int again_re_select_flag;//失败了显示
extern   int state_machine;




extern   int cap_start;
extern   int face_succes_flag;


extern   int x,y;

extern   int face_reconizing; 
extern   int pass_mutex;


extern  int pass_key_num  ;

extern  int pass_success_flag;
// int del_flag = 0;

// int pass_input_key[4] = {0};



extern  int ditance;
extern struct timespec ts;

// extern struct timespec ts2;
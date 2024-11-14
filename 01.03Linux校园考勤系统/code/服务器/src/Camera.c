/**********************************************************************
                 QiJf   2023.08-2023.09

      30HZ CAMERA       捕捉画面，并显示在开发板的LCD上----对图像进行转换
      TCP协议            实现语音控制     server端



        * 注意：当开发板系统重启之后，用户必须重新接入摄像头，否则得不到摄像头的
        * 设备文件  /dev/video7



***********************************************************************/
#include "Project.h"


extern int*lcd_mp;

struct buffer {
        void *                  start;
        size_t                  length;
};
struct buffer * buffers = NULL;
static unsigned int  n_buffers = 0;
static int  camera_fd  = -1; //用来存储打开的摄像头的文件描述符
unsigned int  * rgb32buf;//为了存储转换后的一帧数据



char recv_buf[50] = {0};    //开发板收到的指令

int fd_1_frame;             //捕捉的那一帧人脸

int face_succes_flag = 0;




/******************************摄像头采集函数****************************/

void camera_start()
{
    sleep(1);                                                //让face.jpg延时一会儿
    camera_fd = open ("/dev/video7", O_RDWR | O_NONBLOCK, 0);//O_NONBLOCK参数指的是以非阻塞的形式打开
    init_device();                                           //初始化摄像头
    start_capturing();                                       //开始捕获但是不显示在LCD
    read_JPEG_file("student_face2.jpg",0,0);                 //显示face2.jpg

    pthread_t cam_capture;                 //开启摄像头显示线程
    // pthread_t id_cam_lock;
    int ret5 = pthread_create(&cam_capture,NULL,mainloop,NULL);     //线程--摄像头
    if (ret5) {fprintf(stderr, "线程创建失败%s\n", strerror(ret5)); exit(-1); }

    sleep(3);
    pthread_cancel(cam_capture);



    while(1)
    {
       face_succes_flag = camera_lock();
      if(face_succes_flag ==1)
      {

       read_JPEG_file("success.jpg",0,0);
       break;
      }
    if(face_succes_flag ==0)
      {
       read_JPEG_file("error.jpg",0,0);
       break;
      }

    }

    // if()//密码正确
    // {pthread_cancel(cam_capture);} //mainloop线程中止的
    // else {}//继续显示画面

    //  pthread_exit(NULL);//中止调用这个函数的线程


    sleep(2);
    cap_start = 0;          //关闭摄像头显示-->结束线程
    teacher_ways_flag = 0;  // == 1 就是老师
    student_ways_flag = 0;  // == 1 就是学生
    stu_atten_flag = 0;     //学生打卡方式   1蓝牙 2刷卡  3人脸  4密码
    tea_atten_flag = 0;     //老师打卡方式   1蓝牙 2刷卡  3人脸  4密码
    face_reconizing = 0;    //避免触摸屏冲突 重新清零
    again_re_select_flag = 0;//失败了显示

    desk = 1;
}







//处理采集画面
void *mainloop (void* argv)
{
    struct v4l2_buffer buf;
    unsigned int i;

    //该循环是对摄像头进行监听
    for (;;)
    {
        fd_set fds;
        struct timeval tv;
        int r;

        FD_ZERO (&fds);
        FD_SET (camera_fd, &fds);

        /* Timeout. */
        tv.tv_sec = 2;
        tv.tv_usec = 0;

        select (camera_fd + 1, &fds, NULL, NULL, &tv);


        //循环的把缓冲区进行出队
        for(i = 0; i < n_buffers; ++i) //n_buffers是缓冲区编号
        {
          CLEAR (buf);
          buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
          buf.memory = V4L2_MEMORY_MMAP;

          ioctl (camera_fd, VIDIOC_DQBUF, &buf); //把缓冲区出队

          //对图像进行转换
          yuyv_to_rgb(buffers[i].start);

          label2:

          // if((strncmp(recv_buf, "打开摄像头",4) )== 0 ) {
            if(cap_start == 1){           //不断循环检测是否有开启条件
            camera_to_lcd(70,120);}
            if (cap_start != 1)
            goto label2;
            // else {  read_JPEG_file("student_face2.jpg",0,0);  }//显示face2.jpg
          // }
          // fd_1_frame = open("empty.jpg",O_RDWR|O_CREAT,S_IRWXU); //打开文件，没有则创建它  本文件用于存储...
          //                                                         //捕捉到的那张图片
          // write(fd_1_frame,rgb32buf,sizeof(WIDTH*HEIGHT*3));
          // read_JPEG_file("empty.jpg",70,120);


          ioctl (camera_fd, VIDIOC_QBUF, &buf);

          //  if(face_succes_flag == 1)
          // {
          //   face_succes_flag = 0;
          //  pthread_exit(NULL);//中止调用这个函数的线程
          // }
        }



    }

}




//初始化摄像头模块
void init_device(void)
{

  struct v4l2_format fmt;      //为了设置摄像头捕捉画面的参数

  CLEAR (fmt); //清空结构体

  fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE; //采集视频
  fmt.fmt.pix.width       = WIDTH;       //宽
  fmt.fmt.pix.height      = HEIGHT;      //高
  fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;           //图像格式


  ioctl (camera_fd, VIDIOC_S_FMT, &fmt);

  //申请缓冲区并设置缓冲区参数
  init_mmap();

  rgb32buf  =  (unsigned int  *) malloc( WIDTH*HEIGHT*4 ); //申请堆空间存储RGB格式

}


void init_mmap     (void)
{
  struct v4l2_requestbuffers req;  //申请内存缓冲区

  CLEAR (req);

  req.count               = 4;                            //申请4个
  req.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;  //视频采集
  req.memory              = V4L2_MEMORY_MMAP;             //内存映射

  ioctl (camera_fd, VIDIOC_REQBUFS, &req);

  //为每个图像缓冲区申请内存
  buffers = calloc (req.count, sizeof (*buffers));

  //循环把每个申请的缓冲区都进行参数设置
  for (n_buffers = 0; n_buffers < req.count; ++n_buffers)
  {
      struct v4l2_buffer buf; //设置缓存区参数

      CLEAR (buf);

      buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;  //数据流方式
      buf.memory      = V4L2_MEMORY_MMAP;             //内存映射
      buf.index       = n_buffers;                    //缓冲区编号

      ioctl (camera_fd, VIDIOC_QUERYBUF, &buf);

      buffers[n_buffers].length = buf.length;         //缓冲区的大小
      buffers[n_buffers].start  = mmap (
                                          NULL,
                                          buf.length,
                                          PROT_READ | PROT_WRITE,
                                          MAP_SHARED ,
                                          camera_fd,
                                          buf.m.offset
                                        );

  }
}

//开启视频采集
void start_capturing(void)
{
  unsigned int i;
  enum v4l2_buf_type type;


  for (i = 0; i < n_buffers; ++i)
  {

    struct v4l2_buffer buf;

    CLEAR (buf);

    buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory      = V4L2_MEMORY_MMAP;
    buf.index       = i;

    ioctl (camera_fd, VIDIOC_QBUF, &buf); //把所有的缓冲区入队
  }

  type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

  ioctl (camera_fd, VIDIOC_STREAMON, &type); //发送开启采集指令

}




//转换一个像素点
int yuyv_to_rgb_pix(int Y,int Cb,int Cr)
{
  unsigned int pix_point = 0;//存储一个像素点的数据
  unsigned int R,G,B;      //用来存储RGB颜色分量

  //通过V4L2官方帮助文档中的公式进行计算
  R = (255/219)*(Y - 16) + 1.402 * (127/112)*(Cr - 128);
  G = (255/219)*(Y - 16) - 0.344 * (127/112)*(Cb - 128) - 0.714*(127/112)*(Cr - 128);
  B = (255/219)*(Y - 16) + 1.772 * (127/112)*(Cb - 128);

  //防止越界  RGB分别为占用一个字节 0~255，所以需要进行越界处理
  if(R > 255) R = 255;
  if(G > 255) G = 255;
  if(B > 255) B = 255;
  if(R < 0) R = 0;
  if(G < 0) G = 0;
  if(B < 0) B = 0;

  pix_point = (R<<16)|(G<<8)|B;//把RGB合成一个像素点
  return pix_point;
}



//转换一帧ARGB数据（YUYV转RGB32）
int yuyv_to_rgb(char * yuyvbuf)
{
  int i,j;
  for(i=0,j=0;i<WIDTH*HEIGHT;i+=2,j+=4)
  {
    rgb32buf[i]=yuyv_to_rgb_pix(yuyvbuf[j],yuyvbuf[j+1],yuyvbuf[j+3]);//argb--y1u1y2v1
    rgb32buf[i+1] = yuyv_to_rgb_pix(yuyvbuf[j+2],yuyvbuf[j+1],yuyvbuf[j+3]);
  }
}


//把转换后的数据写到LCD
int  camera_to_lcd(int start_x,int start_y)
{
  int x,y;

  for(y=0;y<HEIGHT;y++)
  {
    for(x=0;x<WIDTH;x++)
    {
      *(lcd_mp+(start_y*800)+start_x + (y*800) + x) = rgb32buf[y*WIDTH+x];
    }
  }
}


/**************************摄像头采集函数END***********************/











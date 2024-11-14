/**********************************************************************
                QiJf   2023.08-2023.09

      void *get_xy(void * arg)                          获取坐标
      void *judge_location(void * arg)                  点击功能分析
      void *read_location_ways_attendence(void * arg)   读取打卡方式坐标
      void lcd_init()                                   LCD显示屏初始化
      int  read_JPEG_file ()                            显示LCD图片

      int  ControlMianUI()                              控制模块,获取坐标并分析
      void open_machine_carton()                        开机动画
      void *state_judge(void * arg)                                根据flag判断状态
      void state_ctrl()                                 各个状态对应得功能

***********************************************************************/
#include "Project.h"

int *lcd_mp =  NULL; //存储内存映射的首地址
int  lcd_fd ; //用于存储LCD的文件代号
int fd_touch;
int x,y;
struct input_event touch;


/*************************状态机标志位****************************************/
int desk = 1;                //是否在桌面 0否 1在
int teacher_ways_flag = 0;   // == 1 就是老师
int student_ways_flag = 0;   // == 1 就是学生
int stu_atten_flag = 0;      //学生打卡方式   1蓝牙 2刷卡  3人脸  4密码
int tea_atten_flag = 0;      //老师打卡方式   1蓝牙 2刷卡  3人脸  4密码
int again_re_select_flag = 0;//失败了显示

/*************************状态机****************************************/
int state_machine = 0;

/*************************一些标志位****************************************/
int face_succs_flag = 0;     //人脸识别标志位  0就是失败 ==1就是 成功




int face_reconizing = 0;      //防止触摸屏冲突
int pass_mutex = 0;          //同上

pthread_t thread_id_desk;                 //桌面动态图




int ControlMianUI()
{
    open_machine_carton();                      //开机动画
    // wave2();
    desk = 1;                                   //在桌面

    pthread_t thread_id_xy_fun;                 // 判断坐标对应的功能
    pthread_t thread_id_xy;                     //用于获取坐标
    pthread_t thread_id_state_judge;            //根据flag判断状态
    pthread_t thread_id_state;
    pthread_t thread_id_server;
    // pthread_t thread_id_wave;
    /*****************************线程创建**********************************/
    int ret1, ret2, ret3, ret4, ret5,ret6,ret7;
    ret2 = pthread_create(&thread_id_xy,NULL,get_xy,NULL);                 //线程--获取坐标
    if (ret2) {fprintf(stderr, "线程创建失败%s\n", strerror(ret2)); exit(-1); }
    ret1 = pthread_create(&thread_id_xy_fun,NULL,judge_location,NULL);     //线程--分析坐标
    if (ret1) {fprintf(stderr, "线程创建失败%s\n", strerror(ret1)); exit(-1); }
    ret4 = pthread_create(&thread_id_state_judge,NULL,state_judge,NULL);   //线程--根据flag判断状态
    if (ret4) {fprintf(stderr, "线程创建失败%s\n", strerror(ret4)); exit(-1); }
    ret3 = pthread_create(&thread_id_state,NULL,state_ctrl,NULL);          //线程--状态机控制
    if (ret3) {fprintf(stderr, "线程创建失败%s\n", strerror(ret3)); exit(-1); }
    ret5 = pthread_create(&thread_id_server,NULL,server_start,NULL);          //线程--服务器
    printf("服务器线程创建成功\n");
    if (ret5) {fprintf(stderr, "线程创建失败%s\n", strerror(ret5)); exit(-1); }
    ret6 = pthread_create(&thread_id_desk,NULL,desk_gif,NULL);                 //线程--桌面
    if (ret6) {fprintf(stderr, "线程创建失败%s\n", strerror(ret6)); exit(-1); }
    // ret7 = pthread_create(&thread_id_wave,NULL,wave,NULL);               //线程--超声波
    // if (ret7) {fprintf(stderr, "线程创建失败%s\n", strerror(ret7)); exit(-1); }


    return 0;
}



void *desk_gif(void * arg)
{
    int i = 0;
    char gif_name[60]={0};
    for(;;)
    {
       label1:
    //    usleep(5000);
        // 显示开机动画 循环显示
        if(desk == 1)
        {

            for(i = 0;i<20;i++)
            {     //把一个固定格式的字符串输出到数组
                sprintf(gif_name,"gdesk%d.jpg",i);
                read_JPEG_file(gif_name,0,0);
                // usleep(50000);
                  nanosleep(&ts, NULL);
                // if(38 == i)
                // i = 0;
                // break;
                 if(desk == 0)
                  goto label1;
            }



        }



    }

}



void *judge_location(void * arg)    // 判断坐标对应的功能
{

    while(1)
        {
            while(1)    /*******************在桌面（状态一）下点击**************************/
            {
                    if((x>490 && x< 730)&&(y>100 && y < 250)&&(desk == 1)&&(pass_mutex ==0))
                                /********选择了老师*********/
                {
                    printf("欢迎老师来上课！请选择打卡方式  坐标是(x=%d,y=%d)\n",x,y);

                    desk = 0;               //不在桌面了
                    teacher_ways_flag = 1;  // == 1 就是老师
                    student_ways_flag = 0;  // == 1 就是学生
                    stu_atten_flag = 0;     //学生打卡方式   1蓝牙 2刷卡  3人脸  4密码
                    tea_atten_flag = 0;     //老师打卡方式   1蓝牙 2刷卡  3人脸  4密码
                    again_re_select_flag = 0;//失败了显示
                    x = 0; y =0;
                    break;
                }
                    if((x>60 && x< 300)&&(y>100 && y < 250)&&(desk == 1))  /********选择了学生*********/
                {
                    printf("欢迎学生来学习！请选择打卡方式  坐标是(x=%d,y=%d)\n",x,y);
                    desk = 0;               //不在桌面了
                    teacher_ways_flag = 0;  // == 1 就是老师
                    student_ways_flag = 1;  // == 1 就是学生
                    stu_atten_flag = 0;     //学生打卡方式   1蓝牙 2刷卡  3人脸  4密码
                    tea_atten_flag = 0;     //老师打卡方式   1蓝牙 2刷卡  3人脸  4密码
                    again_re_select_flag = 0;//失败了显示
                    x = 0; y =0;
                    break;
                }
                break;
            }



            while(1)              /*****************在选择考勤方式下点击******************/
            {
                    if((x>1 && x< 200)&&(y>70 && y < 250)&&(desk == 0)&&
                    (student_ways_flag == 1 || teacher_ways_flag == 1)&&
                    (face_reconizing ==0)&&(pass_mutex == 0))
                {

                    if(student_ways_flag == 1)  {                        /***********选择了刷卡*********/
                    printf("请同学刷卡  坐标是(x=%d,y=%d)\n",x,y);
                    desk = 0;               //不在桌面了
                    teacher_ways_flag = 0;  // == 1 就是老师
                    student_ways_flag = 0;  // == 1 就是学生
                    stu_atten_flag = 2;     //学生打卡方式   1蓝牙 2刷卡  3人脸  4密码
                    tea_atten_flag = 0;     //老师打卡方式   1蓝牙 2刷卡  3人脸  4密码
                    again_re_select_flag = 0;//失败了显示
                    // x = 0; y =0;
                    // break;
                    }

                    else if(teacher_ways_flag == 1){
                    printf("请老师刷卡  坐标是(x=%d,y=%d)\n",x,y);
                    desk = 0;               //不在桌面了
                    teacher_ways_flag = 0;  // == 1 就是老师
                    student_ways_flag = 0;  // == 1 就是学生
                    stu_atten_flag = 0;     //学生打卡方式   1蓝牙 2刷卡  3人脸  4密码
                    tea_atten_flag = 2;     //老师打卡方式   1蓝牙 2刷卡  3人脸  4密码
                    again_re_select_flag = 0;//失败了显示
                    x = 0; y =0;
                    break;
                    }

                }

                    if((x>600 && x< 800)&&(y>70 && y < 250)&&
                    (student_ways_flag == 1 || teacher_ways_flag == 1)&&
                    (face_reconizing ==0)&&(pass_mutex == 0))
                {
                    if(student_ways_flag == 1)  {
                    printf("请同学用蓝牙考勤  坐标是(x=%d,y=%d)\n",x,y);   /**********选择了蓝牙*********/
                    desk = 0;               //不在桌面了
                    teacher_ways_flag = 0;  // == 1 就是老师
                    student_ways_flag = 1;  // == 1 就是学生
                    stu_atten_flag = 1;     //学生打卡方式   1蓝牙 2刷卡  3人脸  4密码
                    tea_atten_flag = 0;     //老师打卡方式   1蓝牙 2刷卡  3人脸  4密码
                    again_re_select_flag = 0;//失败了显示
                    x = 0; y =0;
                    break;
                    }
                    else if(teacher_ways_flag == 1)  {
                    printf("请老师用蓝牙考勤  坐标是(x=%d,y=%d)\n",x,y);
                    desk = 0;               //不在桌面了
                    teacher_ways_flag = 1;  // == 1 就是老师
                    student_ways_flag = 0;  // == 1 就是学生
                    stu_atten_flag = 0;     //学生打卡方式   1蓝牙 2刷卡  3人脸  4密码
                    tea_atten_flag = 1;     //老师打卡方式   1蓝牙 2刷卡  3人脸  4密码
                    again_re_select_flag = 0;//失败了显示
                    x = 0; y =0;
                    break;
                    }

                }

                    if((x>10 && x< 350)&&(y>300 && y < 480)&&
                    (student_ways_flag == 1 || teacher_ways_flag == 1)&&
                    (face_reconizing ==0)&&(pass_mutex == 0))
                    /***********选择了密码*********/
                {
                    if(student_ways_flag == 1)  {
                    printf("请同学用密码考勤  坐标是(x=%d,y=%d)\n",x,y);
                    desk = 0;               //不在桌面了
                    teacher_ways_flag = 0;  // == 1 就是老师
                    student_ways_flag = 1;  // == 1 就是学生
                    stu_atten_flag = 4;     //学生打卡方式   1蓝牙 2刷卡  3人脸  4密码
                    tea_atten_flag = 0;     //老师打卡方式   1蓝牙 2刷卡  3人脸  4密码
                    again_re_select_flag = 0;//失败了显示
                    pass_mutex = 1;         //给密码输入界面的触摸屏解锁
                    x = 0; y =0;
                    break;
                    }
                    else if(teacher_ways_flag == 1){
                    printf("请老师用密码考勤  坐标是(x=%d,y=%d)\n",x,y);
                    desk = 0;               //不在桌面了
                    teacher_ways_flag = 1;  // == 1 就是老师
                    student_ways_flag = 0;  // == 1 就是学生
                    stu_atten_flag = 0;     //学生打卡方式   1蓝牙 2刷卡  3人脸  4密码
                    tea_atten_flag = 4;     //老师打卡方式   1蓝牙 2刷卡  3人脸  4密码
                    pass_mutex = 1;         //给密码输入界面的触摸屏解锁
                    again_re_select_flag = 0;//失败了显示
                    x = 0; y =0;
                    break;
                    }

                }

                    if((x>430 && x< 800)&&(y>300 && y < 480)&&
                    (student_ways_flag == 1 || teacher_ways_flag == 1)&&
                    (face_reconizing ==0)&&(pass_mutex == 0))
                                                                /***********选择了人脸*********/
                {
                    if(student_ways_flag == 1)  {
                    printf("请同学用人脸考勤  坐标是(x=%d,y=%d)\n",x,y);
                    desk = 0;               //不在桌面了
                    teacher_ways_flag = 0;  // == 1 就是老师
                    student_ways_flag = 1;  // == 1 就是学生
                    stu_atten_flag = 3;     //学生打卡方式   1蓝牙 2刷卡  3人脸  4密码
                    tea_atten_flag = 0;     //老师打卡方式   1蓝牙 2刷卡  3人脸  4密码
                    face_reconizing = 1;    //正在识别人脸
                    again_re_select_flag = 0;//失败了显示
                    x = 0; y =0;
                    break;
                    }
                    else if(teacher_ways_flag == 1){
                    printf("请老师用人脸考勤  坐标是(x=%d,y=%d)\n",x,y);
                    desk = 0;               //不在桌面了
                    teacher_ways_flag = 1;  // == 1 就是老师
                    student_ways_flag = 0;  // == 1 就是学生
                    stu_atten_flag = 0;     //学生打卡方式   1蓝牙 2刷卡  3人脸  4密码
                    tea_atten_flag = 3;     //老师打卡方式   1蓝牙 2刷卡  3人脸  4密码
                    face_reconizing = 1;    //正在识别人脸
                    again_re_select_flag = 0;//失败了显示
                    x = 0; y =0;
                    break;
                   }

                }
                break;
            }

            /*事实证明下main的这段话没必要，因为sleep会阻塞进程*/
            /*但是换成了检测就不知道会怎么样了*/

                // while(1)  /*************************在等待考勤界面下点击无效*****************************/
                // {
                //     if((x >=0 && x<=800)&&(y>=0 && y<=480)&&(desk == 0)&&  /*在等待打卡的时候, 触摸屏没有用*/
                //     ((teacher_ways_flag == 1)|| (student_ways_flag == 1))&&
                //     ( stu_atten_flag != 0 || tea_atten_flag != 0)&&(again_re_select_flag == 0))
                //     {
                //         printf("打卡中，点屏幕无效  坐标是(x=%d,y=%d)\n",x,y);
                //         x = 0; y =0;
                //         break;
                //     }
                //     break;
                // }




            while(1) /************************人脸识别界面下点击*********************************/
            {
                if((tea_atten_flag == 3||stu_atten_flag == 3)&&(face_reconizing ==1)&&(pass_mutex ==0))
                {
                    if((x >160 && x<290)&&(y>370 && y<470))      /***点击了开启捕获按钮，摄像头启动***/
                    {
                        printf("摄像头启动，请看向摄像头  坐标是(x=%d,y=%d)\n",x,y);
                        face_succs_flag = 0;      //等于0就是失败 ==1是成功--->点击捕获就刷新一次标志位

                        cap_start = 1;           //使<开启捕获标志位>反转  0是不开启、1是开启

                        x = 0; y =0;
                        break;
                    }

                }


                break;      //并不是break两次的意思
            }







            // while(1) /*******************在again_re_seclet界面下点击 try_again/re-select**********************/
            // {
            //     if((x >230 && x<400)&&(y>340 && y<430)&&(again_re_select_flag == 1)) /***try again***/
            //     {
            //         printf("请重试  坐标是(x=%d,y=%d)\n",x,y);
            //         again_re_select_flag == 0;
            //         x = 0; y =0;
            //         break;

            //     }


            //     if((x >400 && x<600)&&(y>340 && y<430)&&(again_re_select_flag == 1)) /***re-select***/
            //     {
            //         printf("请重新选择打卡方式  坐标是(x=%d,y=%d)\n",x,y);

            //         if(stu_atten_flag != 0)  {
            //         printf("回到学生打卡方式选择界面  坐标是(x=%d,y=%d)\n",x,y);
            //         student_ways_flag = 1;
            //         x = 0; y =0;
            //         break;
            //         }

            //         if(tea_atten_flag != 0)  {
            //         printf("回到老师打卡方式选择界面  坐标是(x=%d,y=%d)\n",x,y);
            //         teacher_ways_flag = 1;
            //         x = 0; y =0;
            //         break;
            //         }


            //     }
            //     break;
            // }


        }

}


void *state_judge(void * arg)
{
    while (1)
    {
        if(desk == 1){
                    teacher_ways_flag = 0;  // == 1 就是老师
                    student_ways_flag = 0;  // == 1 就是学生
                    stu_atten_flag = 0;     //学生打卡方式   1蓝牙 2刷卡  3人脸  4密码
                    tea_atten_flag = 0;     //老师打卡方式   1蓝牙 2刷卡  3人脸  4密码
                    again_re_select_flag = 0;//失败了显示
            state_machine = 1;                                       /*状态一：进入桌面desk*/
        }

        if((teacher_ways_flag == 0) && (student_ways_flag == 1)) {   /*状态二：学生选择考勤方式*/

            state_machine = 2;
        }
        if((teacher_ways_flag == 1) && (student_ways_flag == 0)){
            state_machine = 3;                                       /*状态三：老师选择考勤方式*/
        }



        if((stu_atten_flag == 1))    {                               /*状态4：等待学生蓝牙考勤*/

            state_machine = 4;
        }
        if((stu_atten_flag == 2))    {                               /*状态5：等待学生RFID考勤*/

            state_machine = 5;
        }
        if((stu_atten_flag == 3))    {                               /*状态6：等待学生刷脸考勤*/

            state_machine = 6;
        }
        if((stu_atten_flag == 4))    {                               /*状态7：等待学生密码考勤*/

            state_machine = 7;
        }




        if((tea_atten_flag == 1))    {                               /*状态8：等待老师蓝牙考勤*/

            state_machine = 8;
        }
        if((tea_atten_flag == 2))    {                               /*状态9：等待老师RFID考勤*/

            state_machine = 9;
        }
        if((tea_atten_flag == 3))    {                               /*状态10：等待老师刷脸考勤*/

            state_machine = 10;
        }
        if((tea_atten_flag == 4))    {                               /*状态11：等待老师密码考勤*/

            state_machine = 11;
        }


                                                                     /*状态十二：again_re_select页面*/
        if((again_re_select_flag == 1))    {                         /*状态11：等待老师密码考勤*/

            state_machine = 12;
        }


    }
}




void* state_ctrl(void* arg)
{
while(1)
    {

        switch (state_machine)                      /*******根据状态进行相应的工作**********/
        {

        case 1:                                             /*状态一：进入桌面desk*/
                       //线程--桌面

                //   read_JPEG_file("desk.jpg",0,0);
            break;
        case 2:                                             /*状态二：学生选择考勤方式*/
                read_JPEG_file("student_ways.jpg",0,0);
            break;
        case 3:                                             /*状态三：老师选择考勤方式*/
                read_JPEG_file("teacher_ways.jpg",0,0);
            break;


        case 4:                                             /*状态4：等待学生蓝牙考勤*/
                read_JPEG_file("student_blue.jpg",0,0);
                blue_start();
            break;
        case 5:                                             /*状态5：等待学生RFID考勤*/
                read_JPEG_file("student_swipe.jpg",0,0);
                rfid_start();
            break;
        case 6:                                             /*状态6：等待学生刷脸考勤*/
                read_JPEG_file("student_face.jpg",0,0);
                camera_start();
            break;
        case 7:                                             /*状态7：等待学生密码考勤*/
                read_JPEG_file("student_password.jpg",0,0);
                password_start();
            break;



        case 8:                                             /*状态8：等待老师蓝牙考勤*/
                read_JPEG_file("teacher_blue.jpg",0,0);
                blue_start();
            break;
        case 9:                                             /*状态9：等待老师RFID考勤*/
                read_JPEG_file("teacher_swipe.jpg",0,0);
                rfid_start();
            break;
        case 10:                                             /*状态10：等待老师刷脸考勤*/
                read_JPEG_file("teacher_face.jpg",0,0);
                camera_start();
            break;
        case 11:                                             /*状态11：等待老师密码考勤*/
                read_JPEG_file("teacher_password.jpg",0,0);
                password_start();
            break;



        case 12:                                             /*状态十二：again_re_select页面*/
                read_JPEG_file("again_re_select.jpg",0,0);
            break;
        default: break;
        }

    //    printf("********正在判断应执行哪个状态机********\n");


    }

}







//  获取坐标
void *get_xy(void * arg)
{
      while(1)
    {
        while(1)
        {
            read(fd_touch, &touch ,sizeof(touch));          //不断读取文件
            if(touch.type== EV_ABS && touch.code == ABS_X)   x = touch.value*800/1024;

            if(touch.type== EV_ABS && touch.code== ABS_Y)    y = touch.value*480/600;

            //当按下去的那一刻,跳出循环;只抓取按下去的坐标值
                if(touch.type== EV_KEY && touch.code == BTN_TOUCH &&  touch.value==0) //压力判断
             {

                    printf("（在任何界面下点击都会打印的）坐标 (x=%d,y=%d)\n",x,y);
                    break;

             }

        }
             x = 0; y =0;

    }
}






int lcd_init()
{

    //打开LCD、触摸屏
    lcd_fd = open("/dev/fb0", O_RDWR);
        if (lcd_fd == -1)
        {
            printf("lcd文件打开失败\n");
             return -1;
        }
    fd_touch = open("/dev/input/event0",O_RDWR);
        if(-1 == fd_touch)
        {
            perror("open touch error");
            return -1;
        }

    //内存映射
    lcd_mp = mmap(
    				NULL,
    				800 * 480 * 4 ,
    				PROT_READ | PROT_WRITE,
                    MAP_SHARED ,
                    lcd_fd ,
                    0
                 );
        if (lcd_mp == MAP_FAILED) //成功返回0， 失败返回 -1；（MAP_FAILED）
        {
            perror("mmap failed");
            return -1;
        }

}


 //c语言中字符串是用""修饰的  jpg图片的路径是字符串格式 "xxx.jpg"
int read_JPEG_file (char * filename,int start_x,int start_y)
{

  //创建一个解码对象，用于存储JPEG图像的解码参数
  struct jpeg_decompress_struct cinfo;

  //定义了局部变量
  FILE * infile;			//是一个指针，指向被打开的Jpg文件
  unsigned char * buffer;	//是一个指针，指向申请的一行大小的缓冲区
  int row_stride;			//该变量用于存储计算出来的一行图像的字节大小  一行大小 = 图像宽 * 像素色深 / 8  单位是字节


  //调用fopen打开需要进行解码的jpg图片
  if ((infile = fopen(filename, "rb")) == NULL) {
    fprintf(stderr, "can't open %s\n", filename);
    return 0;
  }

  /* Step 1: 分配并初始化解码对象 */

  //把错误处理对象绑定在解码对象，当解码过程中出现错误，可以进行处理
  struct jpeg_error_mgr jerr;
  cinfo.err = jpeg_std_error(&jerr);

  //初始化解码对象
  jpeg_create_decompress(&cinfo);

  /* Step 2: 指定数据源，目前数据源来自jpg图片 */
  jpeg_stdio_src(&cinfo, infile);

  /* Step 3: 读取jpg文件参数，其实是读取jpg图片的信息*/
  jpeg_read_header(&cinfo, TRUE);

  /* Step 4: 设置解码参数，这个操作是可选的 */

  /* In this example, we don't need to change any of the defaults set by
   * jpeg_read_header(), so we do nothing here.
   */

  /* Step 5: 开始进行解码操作 */
  jpeg_start_decompress(&cinfo);


  //在进行读取jpg图像的像素点之前，需要提前申请存储数据的缓冲区，需要申请内存
  row_stride = cinfo.output_width * cinfo.output_components; //申请的内存大小==图像一行像素的字节大小

  //调用malloc函数来申请内存
  buffer = malloc(row_stride);

  /* Step 6: 循环的从jpg图片中读取像素数据，每次读一行*/

  int i;
  int data; //用于存储一个转换好的像素点数据
  while (cinfo.output_scanline < cinfo.output_height)
  {
   		//调用接口来读取一行解码完成的图像数据  rgb rgb rgb rgb....  jpg(3字节 rgb) --> lcd(4字节 argb)
    	jpeg_read_scanlines(&cinfo, &buffer, 1);

    	//需要把读取的一行jpg图像的颜色数据依次写入到LCD的像素点中  0 1 2  3 4 5  6 7 8
    	for (i = 0; i < cinfo.output_width ; ++i)
    	{
    		data |= buffer[i*3]   << 16; //r
    		data |= buffer[i*3+1] << 8;  //g
    		data |= buffer[i*3+2] ;      //b

    		*(lcd_mp + start_y*800 + start_x + (cinfo.output_scanline-1)*800 + i) = data;  //显示像素点

    		data = 0;
    	}
  }

  /* Step 7: 完成解码操作 */

  jpeg_finish_decompress(&cinfo);

  /* Step 8: 释放解码对象 */

  jpeg_destroy_decompress(&cinfo);

  fclose(infile); //关闭jpg文件

  return 1;
}


void open_machine_carton()
{
      char gif_name[60]={0};
     // 显示开机动画 循环显示
        for(int i = 8;i>1;--i)
       {     //把一个固定格式的字符串输出到数组
         sprintf(gif_name,"/boot/boot%d.jpg",i);
         read_JPEG_file(gif_name,0,0);
          usleep(100000);
       }


}
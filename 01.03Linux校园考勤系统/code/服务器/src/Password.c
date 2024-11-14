#include "Project.h"




int real_pass[5] = {3,8,9,0,'\0'};

int pass_key_num  = 0;

int del_flag = 0;

int pass_input_key[5] = {0,0,0,0,'\0'};

int pass_success_flag =0;

void password_start()
{
      int ret ;
     pass_mutex = 1;
     sleep(1);
     read_JPEG_file("password0.jpg",0,0);

    while(1)
    {
       ret =  passwork_lock();
        if (ret == 1)
        {
         read_JPEG_file("success.jpg",0,0);


         break;
        }

        //   else //失败
        // {
        // read_JPEG_file("error.jpg",0,0);
        // break;
        // }

    }



    sleep(2);
    desk = 1;
    teacher_ways_flag = 0;  // == 1 就是老师
    student_ways_flag = 0;  // == 1 就是学生
    stu_atten_flag = 0;     //学生打卡方式   1蓝牙 2刷卡  3人脸  4密码
    tea_atten_flag = 0;     //老师打卡方式   1蓝牙 2刷卡  3人脸  4密码
    again_re_select_flag = 0;//失败了显示
    state_machine = 0;
    pass_mutex = 0;
    pass_key_num = 0;
    del_flag = 0;
}


int passwork_lock()
{


/************************密码输入界面下点击*********************************/
    for(;;)
    {
          while (1)
        {

            if ((pass_mutex ==1)&&x > 410 && x < 610 && y> 390 && y < 480&&
                    pass_key_num <=3&&
                (face_reconizing ==0))      //按钮0
            {
                printf("input_key0\n");

                pass_key_num++;
                keyshow_pass();
                pass_input_key[pass_key_num] = 0;

                del_flag = 1;

                x = 0; y =0;
                        break;
            }

            if ((pass_mutex ==1)&&x > 215 && x < 410 && y > 100 && y < 195&&
                    pass_key_num <=3)       //按钮1
            {
                printf("input_key1\n");
                 pass_key_num++;
                keyshow_pass();
                pass_input_key[pass_key_num] = 1;

                del_flag = 1;

                x = 0; y =0;
                        break;
            }

            if ((pass_mutex ==1)&&x > 410 && x < 610 && y > 100 && y < 195&&
                    pass_key_num <=3)       //按钮2
            {
                printf("input_key2\n");

                 pass_key_num++;
                  keyshow_pass();
                pass_input_key[pass_key_num] = 2;

                del_flag = 1;

                x = 0; y =0;
                        break;
            }

            if ((pass_mutex ==1)&&x > 610 && x < 800 && y > 100 && y < 195&&
                    pass_key_num <=3)        //按钮3
            {
                printf("input_key3\n");

                 pass_key_num++;
                 keyshow_pass();
                pass_input_key[pass_key_num] = 3;

                del_flag = 1;

                x = 0; y =0;
                        break;
            }

            if ((pass_mutex ==1)&&x > 215 && x < 410 && y > 195 && y < 290 &&
                 pass_key_num <=3)       //按钮4
            {
                printf("input_key4\n");

                 pass_key_num++;
                 keyshow_pass();
                pass_input_key[pass_key_num] = 4;

                del_flag = 1;

                x = 0; y =0;
                        break;
            }

            if ((pass_mutex ==1)&&x > 410 && x < 610 && y > 195 && y < 290&&
                 pass_key_num <=3)       //按钮5
            {
                printf("input_key5\n");

              pass_key_num++;
              keyshow_pass();
                pass_input_key[pass_key_num] = 5;

                del_flag = 1;

                x = 0; y =0;
                        break;
            }

            if ((pass_mutex ==1)&&x > 610 && x < 800 && y > 195 && y < 290
                    &&pass_key_num <=3)       //按纽6
            {
                printf("input_key6\n");
                pass_key_num++;
                 keyshow_pass();
                pass_input_key[pass_key_num] = 6;

                del_flag = 1;

                x = 0; y =0;
                        break;
            }

            if ((pass_mutex ==1)&&x > 215 && x < 410 && y > 290 && y < 390
                    &&pass_key_num <=3)       //按钮7
            {
                printf("input_key7\n");

                pass_key_num++;
                 keyshow_pass();

                pass_input_key[pass_key_num] = 7;

                del_flag = 1;
                x = 0; y =0;
                        break;
            }

            if ((pass_mutex ==1)&&x > 410  && x < 610 && y > 290 && y < 390
                    &&pass_key_num <=3)       //按钮8
            {
                printf("input_key8\n");

               pass_key_num++;
                  keyshow_pass();
                pass_input_key[pass_key_num] = 8;

                del_flag = 1;

                x = 0; y =0;
                        break;
            }

            if ((pass_mutex ==1)&&x > 610 && x < 800 && y > 290 && y < 390
                    &&pass_key_num <=3)       //按钮9
            {
                printf("input_key9\n");

                 pass_key_num++;
                  keyshow_pass();
                pass_input_key[pass_key_num] = 9;

                del_flag = 1;

                x = 0; y =0;
                        break;
            }

            if(pass_key_num == 0 )

            {
                del_flag = 0;
                break;
            }

            // if(pass_key_num == 0 )   del_flag = 0;                //不能再删除了


            if ((pass_mutex ==1)&&x > 610 && x < 800 && y > 390 && y < 480 &&
                    del_flag == 1 &&(pass_key_num > 0))             // 删除键, flag = 1 才有用
            {
                // printf("删除键\n");
                pass_key_num--;
                pass_input_key[pass_key_num]  = 0;
                keyshow_pass();
                printf("删除键key_num == %d\n",pass_key_num);
                x = 0; y =0;
                break;
            }

            // if (pass_key_num == 4) //退出条件
            // {
            //     printf("密码达到4位\n");
            //     break;
            // }&&&&(pass_key_num == 4)   pass_mutex ==1&&(face_reconizing ==0)&& pass_key_num == 4

            if ((pass_mutex ==1)&&x > 215 && x < 410 && y > 390 && y < 480&&(pass_input_key[2] ==8))
                    // ENTER键
            {
                 memset(pass_input_key,0,5);
                 printf("ENTER键\n");
                 printf("密码达到%d位\n",pass_key_num);
                //  pass_success_flag =1;
                // pass_key_num = 0;
                pass_key_num = 0;
                x = 0; y =0;
                return 1;
                break;

               //  pthread_exit(NULL);   //密码输入成功 中止线程

            }

            if((pass_mutex ==1)&&x > 215 && x < 410 && y > 390 && y < 480&&
                    (pass_input_key[2] != 9))
                    {
                      memset(pass_input_key,0,5);
                      read_JPEG_file("error.jpg",0,0);
                      sleep(3);
                      pass_key_num = 0;
                      keyshow_pass() ;

                        break;
                    }


            // if( pass_key_num == 3 )
            // {

            //      break;
            // }

            // break;
        }   //小循环结束

    }
      //大循环结束


        return 1;


}



void keyshow_pass() //输入时的图片切换
    {
        switch (pass_key_num)
        {
        case 0:
            read_JPEG_file("password0.jpg",0,0);
            break;

        case 1:
           read_JPEG_file("password1.jpg",0,0);
            break;

        case 2:
           read_JPEG_file("password2.jpg",0,0);
            break;

        case 3:
            read_JPEG_file("password3.jpg",0,0);
            break;

        case 4:
            read_JPEG_file("password4.jpg",0,0);
            break;

        default:
            break;
        }
    }

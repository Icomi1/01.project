/**********************************************************************
                 QiJf   2023.08-2023.09

       人脸考勤页面下，输入密码解锁



***********************************************************************/
#include "Project.h"

int true_key[4] = {3,8,9,0}; //正确密码
unsigned int key_num = 0;    //输入密码个数: >=0
int input_key[5] = {0};      //输入密码的储存

int panduan = 0;             //最后的判断

int erro_num = 0;            //记录错误次数

int del_work_flag = 0;            //==0删除键无效


int  camera_lock(void )
{

  while (1) //密码输入
   {
            while (1)
        {
            // label1 :panduan = 0;
            if (x > 500 && x < 590 && y > 400 && y < 480&&key_num<=3)      //按钮0
            {
                printf("input_key0\n");
                key_num++;
                keyshow();
                input_key[key_num] = 0;
                del_work_flag = 1;
                x = 0; y =0;
                        break;
            }

            if (x > 500 && x < 590 && y > 150 && y < 230&&key_num<=3)       //按钮1
            {
                printf("input_key1\n");
                key_num++;
                keyshow();
                input_key[key_num] = 1;
                del_work_flag = 1;
                x = 0; y =0;
                        break;
            }

            if (x > 590 && x < 680 && y > 150 && y < 230&&key_num<=3)       //按钮2
            {
                printf("input_key2\n");
                key_num++;
                keyshow();
                input_key[key_num] = 2;
                del_work_flag = 1;
                x = 0; y =0;
                        break;
            }

            if (x > 680 && x < 780 && y > 150 && y < 230&&key_num<=3)        //按钮3
            {
                printf("input_key3\n");
                key_num++;
                keyshow();
                input_key[key_num] = 3;
                del_work_flag = 1;
                x = 0; y =0;
                        break;
            }

            if (x > 500 && x < 590 && y > 230 && y < 313&&key_num<=3)       //按钮4
            {
                printf("input_key4\n");
                key_num++;
                keyshow();
                input_key[key_num] = 4;
                del_work_flag = 1;
                x = 0; y =0;
                        break;
            }

            if (x > 590 && x < 680 && y > 230 && y < 313&&key_num<=3)       //按钮5
            {
                printf("input_key5\n");
                key_num++;
                keyshow();
                input_key[key_num] = 5;
                del_work_flag = 1;
                x = 0; y =0;
                        break;
            }

            if (x > 680 && x < 780 && y > 230 && y < 313&&key_num<=3)       //按纽6
            {
                printf("input_key6\n");
                key_num++;
                keyshow();
                input_key[key_num] = 6;
                del_work_flag = 1;
                x = 0; y =0;
                        break;
            }

            if (x > 500 && x < 590 && y > 313 && y < 390&&key_num<=3)       //按钮7
            {
                printf("input_key7\n");
                key_num++;
                keyshow();
                input_key[key_num] = 7;
                del_work_flag = 1;
                x = 0; y =0;
                        break;
            }

            if (x > 590 && x < 680 && y > 313 && y < 390&&key_num<=3)       //按钮8
            {
                printf("input_key8\n");
                key_num++;
                keyshow();
                input_key[key_num] = 8;
                del_work_flag = 1;
                x = 0; y =0;
                        break;
            }

            if (x > 680 && x < 780 && y > 313 && y < 390&&key_num<=3)       //按钮9
            {
                printf("input_key9\n");
                key_num++;
                keyshow();
                input_key[key_num] = 9;
                del_work_flag = 1;
                x = 0; y =0;
                        break;
            }


            if(key_num == 0 )

            {
                del_work_flag = 0;
                break;
            }


            if (x > 590 && x < 670 && y > 400 && y < 480 && del_work_flag == 1&&(key_num>0))   // 删除键, flag = 1 才有用
            {
                printf("删除键\n");
                key_num--;
                keyshow();
                x = 0; y =0;
                break;
            }

            // if (key_num == 4) //退出条件
            // {
            //     printf("密码达到4位\n");
            //     break;
            // }

            if (x > 670 && x < 800 && y > 400 && y < 480 && (key_num == 4)
                 &&(face_reconizing ==1)&&(pass_mutex == 0) )   // ENTER键
            {
                 printf("ENTER键\n");
                 printf("密码达到%d位\n",key_num);


                // for (panduan = 0; panduan < 4; panduan++)//判断密码正确性
                //     {
                //         if (input_key[panduan] != true_key[panduan])            //密码错误，清空已输入的
                //         {
                //             erro_num += 1;    //错误次数+1
                //             printf("密码错误，错误次数是%d\n",erro_num);
                //             memset(input_key,0,sizeof(input_key));

                //                   //输入的密码清零
                //            key_num = 0;
                //            keyshow();
                //             x = 0; y =0;
                //             break;
                //                                         //一旦有错误跳出当前循环
                //         }

                //     }//&&(input_key[1] == 8)
                memset(input_key,0,5);

                 del_work_flag = 0;            //==0删除键无效
                key_num = 0;
                cap_start == 0;

                x = 0; y =0;

                // if((input_key[0] == true_key[0]))
                return 1;
                // else return 0;


                break;
              //    pthread_exit(NULL);   //密码输入成功 中止线程

            }



        }   //小循环结束

        //   if(erro_num == 3)  {

        //                 sleep(2);

        //                 return 0;

        //                 }


        //     if (panduan == 4)             //正确
        //     {

        //         erro_num = 0;            //错误次数清零

        //         panduan = 0;
        //         face_succes_flag = 1;
        //         cap_start = 0;
                // return 1;

            // }

    }       //大循环结束


}








int keyshow() //输入时的图片切换
    {
        switch (key_num)
        {
        case 0:
            read_JPEG_file("pass_disp0.jpg",470,0);
            break;

        case 1:
           read_JPEG_file("pass_disp1.jpg",470,0);
            break;

        case 2:
           read_JPEG_file("pass_disp2.jpg",470,0);
            break;

        case 3:
            read_JPEG_file("pass_disp3.jpg",470,0);
            break;

        case 4:
            read_JPEG_file("pass_disp4.jpg",470,0);
            break;

        default:
            break;
        }
    }
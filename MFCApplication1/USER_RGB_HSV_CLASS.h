#pragma once
/*
作者 ：guangjie2333
时间 ：2021.10.5
单位 ：SZU
版本 ：V1.0.0
*/

#include <iostream>
#include "USER_DEFINE.h"
using namespace std;


/********************类定义区*********************/

class USER_RGB_HSV_CLASS
{
    public:
        //初始化&退出
        USER_RGB_HSV_CLASS(void);//构造函数声明
        ~USER_RGB_HSV_CLASS(void);//析构函数声明

        //用户自定义函数 
        HSV_STRUCT RGB2HSV(RGB_STRUCT rgb);
        RGB_STRUCT HSV2RGB(HSV_STRUCT hsv);

};


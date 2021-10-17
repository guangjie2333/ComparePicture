#pragma once

/*
作者 ：guangjie2333
时间 ：2021.10.5
单位 ：SZU
版本 ：V1.0.0
*/

#include <wingdi.h> 
#include <afx.h>
using namespace std;

/*************************宏定义区*******************************/

#define WM_GET_DIALOG_HSV_SLIDER_VAL		(WM_USER + 200)            //该变量用于两个窗口之间传递信息


/***********************结构体定义区******************************/

//RGB结构体定义
typedef struct
{
    int r;
    int g;
    int b;
} RGB_STRUCT;

//HSV结构体定义
typedef struct
{
    int h;
    float s;
    float v;
} HSV_STRUCT;

//滚动条数值结构体
typedef struct
{
    int H_slider;
    int S_slider;
    int V_slider;
}HSV_SLIDER_STRUCT;


typedef struct
{
    BYTE* pBmpData;             //图像数据
    BITMAPFILEHEADER bmpHeader; //文件头
    BITMAPINFOHEADER bmpInfo;   //信息头
    CFile bmpFile;              //记录打开文件
}bmpData;
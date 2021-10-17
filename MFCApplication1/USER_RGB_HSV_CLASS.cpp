/*
作者 ：guangjie2333
时间 ：2021.10.5
单位 ：SZU
版本 ：V1.0.0
*/

#include "pch.h"
#include "USER_RGB_HSV_CLASS.h"

// 构造函数
USER_RGB_HSV_CLASS::USER_RGB_HSV_CLASS(void)
{
    cout << "finish by guangjie2333 " << endl;
    cout << "Hardwork makes lucky dog " << endl;
    cout << "please enjoy your life" << endl;
}

// 构造函数
USER_RGB_HSV_CLASS::~USER_RGB_HSV_CLASS(void)
{
    cout << "finish by guangjie2333 " << endl;
    cout << "Hardwork makes lucky dog " << endl;
    cout << "thanks for using my function" << endl;
}


//RGB转换为HSV控空间的函数
// 参照：
//https://www.cnblogs.com/klchang/p/6784856.html

HSV_STRUCT USER_RGB_HSV_CLASS:: RGB2HSV(RGB_STRUCT rgb)
{ 
    // r,g,b values are from 0 to 1
    // h = [0,360], s = [0,1], v = [0,1]

    float R, G, B;
    float min, max, delta, tmp;
    HSV_STRUCT hsv;

    //归一化
    R = (float)rgb.r / 255;
    G = (float)rgb.g / 255;
    B = (float)rgb.b / 255;

    //最小值
    tmp = R > G ? G : R;
    min = tmp > B ? B : tmp;

    //最大值
    tmp = R > G ? R : G;
    max = tmp > B ? tmp : B;

    //max - min 
    delta = max - min;

    //计算v
    hsv.v = max;

    //计算s
    if (max != 0)
    {
        hsv.s = delta / max;
    }
    else
    {
        hsv.s = 0;
    }

    //计算h
    if (0 == hsv.s)
    {
        hsv.h = 0;
    }
    else if (R == max && G >= B)
    {
        hsv.h = (int)((G-B)/delta*60);
    }
    else if (R == max && G < B)
    {
        hsv.h = 360 + (int)((G - B) / delta * 60);
    }
    else if (G == max)
    {
        hsv.h = 120 + (int)((B - R) / delta  * 60) ;
    }
    else if (B == max)
    {
        hsv.h = 240 + (int)((R - G) / delta  * 60);
    }

    //检查范围
    if ((hsv.v >= 0 && hsv.v <= 1) && (hsv.s >= 0 && hsv.s <= 1) && (hsv.h >= 0 && hsv.h <= 360))
    {
        return hsv;
    }
    else
    {
        hsv.h = hsv.h % 360;
        hsv.s = hsv.s > 1 ? 1 : hsv.s;
        hsv.v = hsv.v > 1 ? 1 : hsv.v;
        return hsv;
    }
}


//RGB转换为HSV空间的函数
// 参照：
//雷老师的ppt 
/*
    需要注意的是老师的ppt有问题 ，结果没 * 255 ，花了我两个晚上，我还一直以为是我代码的问题。
    而且ppt还有一个问题 ： f = H/60 - i;  而不是 f = f % 60;
    总之，我花了很多时间去找bug，最后发现转换公式这个最基本的地方出了问题
*/


RGB_STRUCT USER_RGB_HSV_CLASS::HSV2RGB(HSV_STRUCT hsv)
{
    int H= hsv.h%360;
    float S = hsv.s > 1 ? 1 : hsv.s;
    float V = hsv.v > 1 ? 1 : hsv.v;
    float p, q, t;
    int i, f;
    RGB_STRUCT rgb = {0,0,0};

    i = H / 60;
    f = H/60 - i;

    p = V * (1 - S);
    q = V * (1 - S * f);
    t = V * (1 - S * (1 - f));

    switch (i)
    {
    case 0:
        rgb.r = V * 255;
        rgb.g = t * 255;
        rgb.b = p * 255;
        break;
    case 1:
        rgb.r = q * 255;
        rgb.g = V * 255;
        rgb.b = p * 255;
        break;
    case 2:
        rgb.r = p * 255;
        rgb.g = V * 255;
        rgb.b = t * 255;
        break;
    case 3:
        rgb.r = p * 255;
        rgb.g = q * 255;
        rgb.b = V * 255;
        break;
    case 4:
        rgb.r = t * 255;
        rgb.g = p * 255;
        rgb.b = V * 255;
        break;
    case 5:
        rgb.r = V * 255;
        rgb.g = p * 255;
        rgb.b = q * 255;
        break;
    default:
        break;
    }


    return rgb;

}
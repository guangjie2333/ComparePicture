#include "pch.h"
#include "COMPARE_PICTURE.h"

#include <stdio.h>
#include <tchar.h>

//WINAPI 拷贝指定的hDC的lpRect部分到文件Dstfile中(BMP文件格式)
//该代码摘自https://blog.csdn.net/zgl7903/article/details/7831758
BOOL COMPARE_PICTURE :: SaveHDCToFile(HDC hDC, LPRECT lpRect, LPCTSTR Dstfile)
{
    int Width = lpRect->right - lpRect->left;
    int Height = lpRect->bottom - lpRect->top;

    HDC memDC = CreateCompatibleDC(hDC);//内存DC 
    HBITMAP memBitmap = CreateCompatibleBitmap(hDC, Width, Height); //建立和屏幕兼容的bitmap  
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(memDC, memBitmap);//将memBitmap选入内存DC 
    BitBlt(memDC, 0, 0, Width, Height,
        hDC, lpRect->left, lpRect->top,
        SRCCOPY);//复制屏幕图像到内存DC 

      //以下代码保存memDC中的位图到文件 
    BITMAP bmpInfo;
    GetObject(memBitmap, sizeof(bmpInfo), &bmpInfo);//获得位图信息 

    DWORD bmpBytesSize = bmpInfo.bmWidthBytes * bmpInfo.bmHeight;

    BITMAPFILEHEADER bfh = { 0 };//位图文件头 
    bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);//到位图数据的偏移量 
    bfh.bfSize = bfh.bfOffBits + bmpBytesSize;//文件总的大小 
    bfh.bfType = (WORD)0x4d42;

    BITMAPINFOHEADER bih = { 0 };//位图信息头 
    bih.biBitCount = bmpInfo.bmBitsPixel;//每个像素字节大小 
    bih.biCompression = BI_RGB;
    bih.biHeight = bmpInfo.bmHeight;//高度 
    bih.biPlanes = 1;
    bih.biSize = sizeof(BITMAPINFOHEADER);
    bih.biSizeImage = bmpBytesSize;//图像数据大小 
    bih.biWidth = bmpInfo.bmWidth;//宽度  

    BYTE* p = new BYTE[bmpBytesSize];//申请内存保存位图数据  
    GetDIBits(memDC, memBitmap, 0, Height,
        p, (LPBITMAPINFO)&bih, DIB_RGB_COLORS);//获取位图数据 

      //正常的还要对256及以下的颜色加颜色映射表，
      // 但是现在的显示模式基本上都是真彩（24或32位）显示，因此忽略此项

     //保存到文件
    HANDLE hFile = CreateFile(Dstfile,
        GENERIC_WRITE,
        0, NULL,
        CREATE_ALWAYS,
        0,
        NULL);
    if (hFile != INVALID_HANDLE_VALUE)
    {
        DWORD dwBytesWritten;

        WriteFile(hFile, &bfh, sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL);//写入位图文件头 
        WriteFile(hFile, &bih, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);//写入位图信息头 
        WriteFile(hFile, p, bmpInfo.bmWidthBytes * bmpInfo.bmHeight, &dwBytesWritten, NULL);//写入位图数据 
        CloseHandle(hFile);
    }
    else
    {
        TCHAR szMsg[256];
        _stprintf_s(szMsg, _T("CreateFile error=%lu"), GetLastError());
        MessageBox(NULL, szMsg, _T("Error"), MB_OK);
    }


    //结束清理工作
    delete[] p;

    SelectObject(memDC, hOldBitmap);
    DeleteObject(memBitmap);
    DeleteDC(memDC);

    return(true);
}

//将上述代码为我所用
int COMPARE_PICTURE::ComparePicture(HDC hDC1, LPRECT lpRect1, HDC hDC2, LPRECT lpRect2)
{
    /***********************************************

    处理第一幅图

    *************************************************/

    int Width1 = lpRect1->right - lpRect1->left;
    int Height1 = lpRect1->bottom - lpRect1->top;

    HDC memDC1 = CreateCompatibleDC(hDC1);//内存DC 
    HBITMAP memBitmap1 = CreateCompatibleBitmap(hDC1, Width1, Height1); //建立和屏幕兼容的bitmap  
    HBITMAP hOldBitmap1 = (HBITMAP)SelectObject(memDC1, memBitmap1);//将memBitmap选入内存DC 
    BitBlt(memDC1, 0, 0, Width1, Height1,
        hDC1, lpRect1->left, lpRect1->top,
        SRCCOPY);//复制屏幕图像到内存DC 

      //以下代码保存memDC中的位图到文件 
    BITMAP bmpInfo1;
    GetObject(memBitmap1, sizeof(bmpInfo1), &bmpInfo1);//获得位图信息 

    DWORD bmpBytesSize1 = bmpInfo1.bmWidthBytes * bmpInfo1.bmHeight * 3;

    BITMAPFILEHEADER bfh1 = { 0 };//位图文件头 
    bfh1.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);//到位图数据的偏移量 
    bfh1.bfSize = bfh1.bfOffBits + bmpBytesSize1;//文件总的大小 
    bfh1.bfType = (WORD)0x4d42;

    BITMAPINFOHEADER bih1 = { 0 };//位图信息头 
    bih1.biBitCount = bmpInfo1.bmBitsPixel;//每个像素字节大小 
    bih1.biCompression = BI_RGB;
    bih1.biHeight = bmpInfo1.bmHeight;//高度 
    bih1.biPlanes = 1;
    bih1.biSize = sizeof(BITMAPINFOHEADER);
    bih1.biSizeImage = bmpBytesSize1;//图像数据大小 
    bih1.biWidth = bmpInfo1.bmWidth;//宽度  

    BYTE* p1 = new BYTE[bmpBytesSize1];//申请内存保存位图数据  
    GetDIBits(memDC1, memBitmap1, 0, Height1,
        p1, (LPBITMAPINFO)&bih1, DIB_RGB_COLORS);//获取位图数据 

    /***********************************************
    
    处理第二幅图

    *************************************************/

    int Width2 = lpRect2->right - lpRect2->left;
    int Height2 = lpRect2->bottom - lpRect2->top;

    HDC memDC2 = CreateCompatibleDC(hDC2);//内存DC 
    HBITMAP memBitmap2 = CreateCompatibleBitmap(hDC2, Width2, Height2); //建立和屏幕兼容的bitmap  
    HBITMAP hOldBitmap2 = (HBITMAP)SelectObject(memDC2, memBitmap2);//将memBitmap选入内存DC 
    BitBlt(memDC2, 0, 0, Width2, Height2,
        hDC2, lpRect2->left, lpRect2->top,
        SRCCOPY);//复制屏幕图像到内存DC 

      //以下代码保存memDC中的位图到文件 
    BITMAP bmpInfo2;
    GetObject(memBitmap2, sizeof(bmpInfo2), &bmpInfo2);//获得位图信息 

    DWORD bmpBytesSize2 = bmpInfo2.bmWidthBytes * bmpInfo2.bmHeight * 3;

    BITMAPFILEHEADER bfh2 = { 0 };//位图文件头 
    bfh2.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);//到位图数据的偏移量 
    bfh2.bfSize = bfh2.bfOffBits + bmpBytesSize2;//文件总的大小 
    bfh2.bfType = (WORD)0x4d42;

    BITMAPINFOHEADER bih2 = { 0 };//位图信息头 
    bih2.biBitCount = bmpInfo2.bmBitsPixel;//每个像素字节大小 
    bih2.biCompression = BI_RGB;
    bih2.biHeight = bmpInfo2.bmHeight;//高度 
    bih2.biPlanes = 1;
    bih2.biSize = sizeof(BITMAPINFOHEADER);
    bih2.biSizeImage = bmpBytesSize2;//图像数据大小 
    bih2.biWidth = bmpInfo2.bmWidth;//宽度  

    BYTE* p2 = new BYTE[bmpBytesSize2];//申请内存保存位图数据  
    GetDIBits(memDC2, memBitmap2, 0, Height2,
        p2, (LPBITMAPINFO)&bih2, DIB_RGB_COLORS);//获取位图数据 

      //正常的还要对256及以下的颜色加颜色映射表，
      // 但是现在的显示模式基本上都是真彩（24或32位）显示，因此忽略此项

    /***********************************************

    相似性计算

    *************************************************/

    int diffSum = 0; //一个记录像素之差的变量

    if (bmpBytesSize2 == bmpBytesSize1)
    {
        for (int i = 0; i < bmpBytesSize1; i = i + 3)
        {
            int ave; //ave 值越小就越相似
            ave = (abs(p1[i + 0] - p2[i + 0]) + abs(p1[i + 1] - p2[i + 1]) + abs(p1[i + 2] - p2[i + 2])) / 3;
            diffSum = ave + diffSum;
        }

    }
    else
    {
        TRACE("\n\n  两幅图大小不一致，不能比较  \n\n");
    }


    /***********************************************

    结束清理工作

    *************************************************/

    delete[] p1;

    SelectObject(memDC1, hOldBitmap1);
    DeleteObject(memBitmap1);
    DeleteDC(memDC1);

    delete[] p2;

    SelectObject(memDC2, hOldBitmap2);
    DeleteObject(memBitmap2);
    DeleteDC(memDC2);

    return(diffSum);
}
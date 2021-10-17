#include "pch.h"
#include "COMPARE_PICTURE.h"

#include <stdio.h>
#include <tchar.h>

//WINAPI ����ָ����hDC��lpRect���ֵ��ļ�Dstfile��(BMP�ļ���ʽ)
//�ô���ժ��https://blog.csdn.net/zgl7903/article/details/7831758
BOOL COMPARE_PICTURE :: SaveHDCToFile(HDC hDC, LPRECT lpRect, LPCTSTR Dstfile)
{
    int Width = lpRect->right - lpRect->left;
    int Height = lpRect->bottom - lpRect->top;

    HDC memDC = CreateCompatibleDC(hDC);//�ڴ�DC 
    HBITMAP memBitmap = CreateCompatibleBitmap(hDC, Width, Height); //��������Ļ���ݵ�bitmap  
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(memDC, memBitmap);//��memBitmapѡ���ڴ�DC 
    BitBlt(memDC, 0, 0, Width, Height,
        hDC, lpRect->left, lpRect->top,
        SRCCOPY);//������Ļͼ���ڴ�DC 

      //���´��뱣��memDC�е�λͼ���ļ� 
    BITMAP bmpInfo;
    GetObject(memBitmap, sizeof(bmpInfo), &bmpInfo);//���λͼ��Ϣ 

    DWORD bmpBytesSize = bmpInfo.bmWidthBytes * bmpInfo.bmHeight;

    BITMAPFILEHEADER bfh = { 0 };//λͼ�ļ�ͷ 
    bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);//��λͼ���ݵ�ƫ���� 
    bfh.bfSize = bfh.bfOffBits + bmpBytesSize;//�ļ��ܵĴ�С 
    bfh.bfType = (WORD)0x4d42;

    BITMAPINFOHEADER bih = { 0 };//λͼ��Ϣͷ 
    bih.biBitCount = bmpInfo.bmBitsPixel;//ÿ�������ֽڴ�С 
    bih.biCompression = BI_RGB;
    bih.biHeight = bmpInfo.bmHeight;//�߶� 
    bih.biPlanes = 1;
    bih.biSize = sizeof(BITMAPINFOHEADER);
    bih.biSizeImage = bmpBytesSize;//ͼ�����ݴ�С 
    bih.biWidth = bmpInfo.bmWidth;//���  

    BYTE* p = new BYTE[bmpBytesSize];//�����ڴ汣��λͼ����  
    GetDIBits(memDC, memBitmap, 0, Height,
        p, (LPBITMAPINFO)&bih, DIB_RGB_COLORS);//��ȡλͼ���� 

      //�����Ļ�Ҫ��256�����µ���ɫ����ɫӳ���
      // �������ڵ���ʾģʽ�����϶�����ʣ�24��32λ����ʾ����˺��Դ���

     //���浽�ļ�
    HANDLE hFile = CreateFile(Dstfile,
        GENERIC_WRITE,
        0, NULL,
        CREATE_ALWAYS,
        0,
        NULL);
    if (hFile != INVALID_HANDLE_VALUE)
    {
        DWORD dwBytesWritten;

        WriteFile(hFile, &bfh, sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL);//д��λͼ�ļ�ͷ 
        WriteFile(hFile, &bih, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);//д��λͼ��Ϣͷ 
        WriteFile(hFile, p, bmpInfo.bmWidthBytes * bmpInfo.bmHeight, &dwBytesWritten, NULL);//д��λͼ���� 
        CloseHandle(hFile);
    }
    else
    {
        TCHAR szMsg[256];
        _stprintf_s(szMsg, _T("CreateFile error=%lu"), GetLastError());
        MessageBox(NULL, szMsg, _T("Error"), MB_OK);
    }


    //����������
    delete[] p;

    SelectObject(memDC, hOldBitmap);
    DeleteObject(memBitmap);
    DeleteDC(memDC);

    return(true);
}

//����������Ϊ������
int COMPARE_PICTURE::ComparePicture(HDC hDC1, LPRECT lpRect1, HDC hDC2, LPRECT lpRect2)
{
    /***********************************************

    �����һ��ͼ

    *************************************************/

    int Width1 = lpRect1->right - lpRect1->left;
    int Height1 = lpRect1->bottom - lpRect1->top;

    HDC memDC1 = CreateCompatibleDC(hDC1);//�ڴ�DC 
    HBITMAP memBitmap1 = CreateCompatibleBitmap(hDC1, Width1, Height1); //��������Ļ���ݵ�bitmap  
    HBITMAP hOldBitmap1 = (HBITMAP)SelectObject(memDC1, memBitmap1);//��memBitmapѡ���ڴ�DC 
    BitBlt(memDC1, 0, 0, Width1, Height1,
        hDC1, lpRect1->left, lpRect1->top,
        SRCCOPY);//������Ļͼ���ڴ�DC 

      //���´��뱣��memDC�е�λͼ���ļ� 
    BITMAP bmpInfo1;
    GetObject(memBitmap1, sizeof(bmpInfo1), &bmpInfo1);//���λͼ��Ϣ 

    DWORD bmpBytesSize1 = bmpInfo1.bmWidthBytes * bmpInfo1.bmHeight * 3;

    BITMAPFILEHEADER bfh1 = { 0 };//λͼ�ļ�ͷ 
    bfh1.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);//��λͼ���ݵ�ƫ���� 
    bfh1.bfSize = bfh1.bfOffBits + bmpBytesSize1;//�ļ��ܵĴ�С 
    bfh1.bfType = (WORD)0x4d42;

    BITMAPINFOHEADER bih1 = { 0 };//λͼ��Ϣͷ 
    bih1.biBitCount = bmpInfo1.bmBitsPixel;//ÿ�������ֽڴ�С 
    bih1.biCompression = BI_RGB;
    bih1.biHeight = bmpInfo1.bmHeight;//�߶� 
    bih1.biPlanes = 1;
    bih1.biSize = sizeof(BITMAPINFOHEADER);
    bih1.biSizeImage = bmpBytesSize1;//ͼ�����ݴ�С 
    bih1.biWidth = bmpInfo1.bmWidth;//���  

    BYTE* p1 = new BYTE[bmpBytesSize1];//�����ڴ汣��λͼ����  
    GetDIBits(memDC1, memBitmap1, 0, Height1,
        p1, (LPBITMAPINFO)&bih1, DIB_RGB_COLORS);//��ȡλͼ���� 

    /***********************************************
    
    ����ڶ���ͼ

    *************************************************/

    int Width2 = lpRect2->right - lpRect2->left;
    int Height2 = lpRect2->bottom - lpRect2->top;

    HDC memDC2 = CreateCompatibleDC(hDC2);//�ڴ�DC 
    HBITMAP memBitmap2 = CreateCompatibleBitmap(hDC2, Width2, Height2); //��������Ļ���ݵ�bitmap  
    HBITMAP hOldBitmap2 = (HBITMAP)SelectObject(memDC2, memBitmap2);//��memBitmapѡ���ڴ�DC 
    BitBlt(memDC2, 0, 0, Width2, Height2,
        hDC2, lpRect2->left, lpRect2->top,
        SRCCOPY);//������Ļͼ���ڴ�DC 

      //���´��뱣��memDC�е�λͼ���ļ� 
    BITMAP bmpInfo2;
    GetObject(memBitmap2, sizeof(bmpInfo2), &bmpInfo2);//���λͼ��Ϣ 

    DWORD bmpBytesSize2 = bmpInfo2.bmWidthBytes * bmpInfo2.bmHeight * 3;

    BITMAPFILEHEADER bfh2 = { 0 };//λͼ�ļ�ͷ 
    bfh2.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);//��λͼ���ݵ�ƫ���� 
    bfh2.bfSize = bfh2.bfOffBits + bmpBytesSize2;//�ļ��ܵĴ�С 
    bfh2.bfType = (WORD)0x4d42;

    BITMAPINFOHEADER bih2 = { 0 };//λͼ��Ϣͷ 
    bih2.biBitCount = bmpInfo2.bmBitsPixel;//ÿ�������ֽڴ�С 
    bih2.biCompression = BI_RGB;
    bih2.biHeight = bmpInfo2.bmHeight;//�߶� 
    bih2.biPlanes = 1;
    bih2.biSize = sizeof(BITMAPINFOHEADER);
    bih2.biSizeImage = bmpBytesSize2;//ͼ�����ݴ�С 
    bih2.biWidth = bmpInfo2.bmWidth;//���  

    BYTE* p2 = new BYTE[bmpBytesSize2];//�����ڴ汣��λͼ����  
    GetDIBits(memDC2, memBitmap2, 0, Height2,
        p2, (LPBITMAPINFO)&bih2, DIB_RGB_COLORS);//��ȡλͼ���� 

      //�����Ļ�Ҫ��256�����µ���ɫ����ɫӳ���
      // �������ڵ���ʾģʽ�����϶�����ʣ�24��32λ����ʾ����˺��Դ���

    /***********************************************

    �����Լ���

    *************************************************/

    int diffSum = 0; //һ����¼����֮��ı���

    if (bmpBytesSize2 == bmpBytesSize1)
    {
        for (int i = 0; i < bmpBytesSize1; i = i + 3)
        {
            int ave; //ave ֵԽС��Խ����
            ave = (abs(p1[i + 0] - p2[i + 0]) + abs(p1[i + 1] - p2[i + 1]) + abs(p1[i + 2] - p2[i + 2])) / 3;
            diffSum = ave + diffSum;
        }

    }
    else
    {
        TRACE("\n\n  ����ͼ��С��һ�£����ܱȽ�  \n\n");
    }


    /***********************************************

    ����������

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
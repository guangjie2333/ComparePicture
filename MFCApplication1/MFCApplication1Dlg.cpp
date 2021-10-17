
// MFCApplication1Dlg.cpp: 实现文件

/*
作者 ：guangjie2333
时间 ：2021.10.5
单位 ：SZU
版本 ：V1.0.0
*/

#include "pch.h"
#include "framework.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/**************自定义的类调用**************/

#include  "USER_BAR_CLASS_DLG.h"
#include "USER_RGB_HSV_CLASS.h"
#include "COMPARE_PICTURE.h"
//#include "stdlib.h"

/***************内部变量声明***************/

bmpData bmpdata;
CString BmpName;
CString EntName;

/***************内部函数声明***************/
void Cal_HSV_Scale(HSV_SLIDER_STRUCT hsv_slider_struct,float *hScale, float* sScale, float* vScale);  //调节滚动条实际上是在调节缩放系数


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCApplication1Dlg 对话框



CMFCApplication1Dlg::CMFCApplication1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION1_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OPENBMP, &CMFCApplication1Dlg::OnBnClickedButtonOpenbmp)
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BUTTON_HSV2RGB, &CMFCApplication1Dlg::OnBnClickedButtonHsv2rgb)
	ON_MESSAGE(WM_GET_DIALOG_HSV_SLIDER_VAL, UserMessageHandler) //用户自定义的消息标识和函数的绑定
	ON_BN_CLICKED(IDC_BUTTON_OPENBMP2, &CMFCApplication1Dlg::OnBnClickedButtonOpenbmp2)
	ON_BN_CLICKED(IDC_BUTTON_COMPARE, &CMFCApplication1Dlg::OnBnClickedButtonCompare)
END_MESSAGE_MAP()


// CMFCApplication1Dlg 消息处理程序

BOOL CMFCApplication1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCApplication1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCApplication1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCApplication1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



//读取图片，readBmp按键按下响应函数

void CMFCApplication1Dlg::OnBnClickedButtonOpenbmp()
{
	// TODO: 在此添加控件通知处理程序代码
	
	//打开文件 
	CString filter = (CString)"图像文件(*.bmp)|*.bmp;*.BMP||";//指明可供选择的文件类型和相应的扩展名
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter, NULL);  //打开文件

//按下确定按钮 dlg.DoModal() 函数显示对话框
	if (dlg.DoModal() == IDOK)
	{
		//打开对话框获取图像信息
		BmpName = dlg.GetPathName();     //获取文件路径名   
	    EntName = dlg.GetFileExt();      //获取文件扩展名
		EntName.MakeLower();                     //将文件扩展名转换为一个小写字符

		if (EntName.Compare(_T("bmp")) == 0)  //如果是bmp图片则打开显示
		{
			//定义变量存储图片信息
			BITMAPINFO* pBmpInfo;       //记录图像信息头内容
			

				//以只读的方式打开文件 读取bmp图片各部分 bmp文件头 信息 数据
			if (!bmpdata.bmpFile.Open(BmpName, CFile::modeRead | CFile::typeBinary))
				return;
			if (bmpdata.bmpFile.Read(&bmpdata.bmpHeader, sizeof(BITMAPFILEHEADER)) != sizeof(BITMAPFILEHEADER))
				return;
			if (bmpdata.bmpFile.Read(&bmpdata.bmpInfo, sizeof(BITMAPINFOHEADER)) != sizeof(BITMAPINFOHEADER))
				return;
			pBmpInfo = (BITMAPINFO*)new char[sizeof(BITMAPINFOHEADER)];
			//为图像数据申请空间
			memcpy(pBmpInfo, &bmpdata.bmpInfo, sizeof(BITMAPINFOHEADER));  //存储图像信息头内容
			DWORD dataBytes = bmpdata.bmpHeader.bfSize - bmpdata.bmpHeader.bfOffBits;//图像数据大小，单位为字节
			bmpdata.pBmpData = (BYTE*)new char[dataBytes];
			bmpdata.bmpFile.Seek(bmpdata.bmpHeader.bfOffBits,0);//这一步非常重要，必须要把文件指针偏移
			bmpdata.bmpFile.Read(bmpdata.pBmpData, dataBytes);  //存储图像数据(以文件指针为起点开始读dataBytes个数据)
			bmpdata.bmpFile.Close();

			//显示图像1	
			CWnd* pWnd = GetDlgItem(IDC_STATIC_PICTURE); //获得pictrue控件窗口的句柄			
			CRect rect;
			pWnd->GetClientRect(&rect); //获得pictrue控件所在的矩形区域			
			CDC* pDC = pWnd->GetDC(); //获得pictrue控件的DC			
			pDC->SetStretchBltMode(COLORONCOLOR);
			StretchDIBits(pDC->GetSafeHdc(), 0, 0, rect.Width(), rect.Height(), 0, 0, bmpdata.bmpInfo.biWidth, bmpdata.bmpInfo.biHeight, bmpdata.pBmpData, pBmpInfo, DIB_RGB_COLORS, SRCCOPY);
			
			//打印信息
			TRACE(" rect.Width() = %d , rect.Height() = %d, bmpInfo.biWidth = %d ,  bmpInfo.biHeight = %d \n\n",rect.Width(), rect.Height(), bmpdata.bmpInfo.biWidth, bmpdata.bmpInfo.biHeight);
			

		}
	}
}


//鼠标在屏幕中移动的事件响应函数
void CMFCApplication1Dlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//guangjie2333的设计
	//需要注意的是图像存在伸缩，需要更具伸缩比例确定

	CDialogEx::OnMouseMove(nFlags, point);

	CRect pect;

	CWnd* pWnd = GetDlgItem(IDC_STATIC_PICTURE);//IDC_PICTURE为控件ID号

	pWnd->GetClientRect(&pect);

	int high = pect.Height();   //返回高

	int width = pect.Width();   //返回宽


	TRACE("picture 控件长宽高信息  high = %d  width = %d \n\n" , high, width);


	//确保鼠标在图像内移动
	if (1)
	{
		TRACE("捕捉到了鼠标移动，当前位置 X = %d  Y = %d \n\n", point.x, point.y);
		SetDlgItemInt(IDC_EDIT_X, point.x);			 //写入坐标值x
		SetDlgItemInt(IDC_EDIT_Y, point.y);			 //写入坐标值y

		HDC hDC = ::GetDC(NULL);
		GetCursorPos(&point);
		COLORREF rgb = ::GetPixel(hDC, point.x, point.y );  //后两个参数是相对坐标
		


		RGB_STRUCT rgbStruct;
		rgbStruct.r = GetRValue(rgb);			    //获得灰度分量
		rgbStruct.g = GetGValue(rgb);
		rgbStruct.b = GetBValue(rgb);
		SetDlgItemInt(IDC_EDIT_R, rgbStruct.r);     //写入灰度分量R
		SetDlgItemInt(IDC_EDIT_G, rgbStruct.g);     //写入灰度分量G
		SetDlgItemInt(IDC_EDIT_B, rgbStruct.b);     //写入灰度分量B

		//显示该点的hsv值
		USER_RGB_HSV_CLASS userClass_rgb_hsv;       //用户自定义的 hsv—rgb 转换类
		HSV_STRUCT hsvStruct;
		hsvStruct = userClass_rgb_hsv.RGB2HSV(rgbStruct);

		CString str;
		SetDlgItemInt(IDC_EDIT_H, hsvStruct.h);     //写入灰度分量R
		str.Format(_T("%.5f"), hsvStruct.s);
		SetDlgItemText(IDC_EDIT_S, str);			//写入灰度分量G
		str.Format(_T("%.5f"), hsvStruct.v);
		SetDlgItemText(IDC_EDIT_V, str);			//写入灰度分量B

	}
	else
	{
		SetDlgItemInt(IDC_EDIT_X, 0);				//写入坐标值x
		SetDlgItemInt(IDC_EDIT_Y, 0);				//写入坐标值y
		SetDlgItemInt(IDC_EDIT_R, 0);				//写入灰度分量R
		SetDlgItemInt(IDC_EDIT_G, 0);				//写入灰度分量G
		SetDlgItemInt(IDC_EDIT_B, 0);				//写入灰度分量B
		SetDlgItemInt(IDC_EDIT_H, 0);				//写入灰度分量H
		SetDlgItemInt(IDC_EDIT_S, 0);				//写入灰度分量S
		SetDlgItemInt(IDC_EDIT_V, 0);				//写入灰度分量V
	}
}


//rgb2hsv按键按下响应函数
void CMFCApplication1Dlg::OnBnClickedButtonHsv2rgb()
{
	// TODO: 在此添加控件通知处理程序代码
	
	// guangjie2333的设计
	
	// 按键按下后做两件事 1.读取图像 2.打开新的对话框

	//将按键按下和新的Dialog联系
	USER_BAR_CLASS_DLG dlg;
	dlg.phwnd = m_hWnd;
	dlg.DoModal();


	                                   
}

//用户自定义的消息处理函数
LRESULT CMFCApplication1Dlg::UserMessageHandler(WPARAM w, LPARAM l)
{
	// guangjie2333的设计
	((CStatic*)GetDlgItem(IDC_STATIC_PICTURE2))->SetBitmap(NULL);	//清除原有图像

	/*按理说 WPARAM w应该是一个实际值，但是我通过指针转换的方式传入了地址
	  现在拿到了地址后，我有两件事情要做 ： 
	  1. 把地址的解析方式从(int) 改成 (HSV_SLIDER_STRUCT*)
	  2. 把结构体中的值取出来
	  */
	HSV_SLIDER_STRUCT hsv_slider_val = *(HSV_SLIDER_STRUCT*)w;

	float hScale, sScale, vScale;
	int r, g, b;

	/*为了避免对原数据进行修改，新建一个图像存储结构体
	  
	  之后需要做的就是 ：
	  1.将原数据拷贝一份
	  2.将原rgb数据转换成hsv数据
	  3.再将hsv数据转换成rgb显示
	  
	  需要注意的是：1.我默认bmp图像是真彩色，24bit ，B G R三通道，不带保留项的
					2.图像是每三个数据构成一个像素点，按 B G R顺序组合而成的
					3.适用大部分的bmp图像
	  
	  */

	USER_RGB_HSV_CLASS userClass_rgb_hsv;                           //用户自定义的 hsv—rgb 转换类
	HSV_STRUCT hsv_struct;                                          //用户自定义的 hsv结构体
	RGB_STRUCT rgb_struct;											//用户自定义的 rgb结构体

	//拷贝原数据
	DWORD dataBytes = bmpdata.bmpHeader.bfSize - bmpdata.bmpHeader.bfOffBits;//图像数据大小，单位为字节
	BYTE* pixelArray = (BYTE*)new char[dataBytes];
	memcpy(pixelArray, bmpdata.pBmpData, dataBytes);

	Cal_HSV_Scale(hsv_slider_val, &hScale, &sScale, &vScale);      //计算缩放系数

	//逐个像素进行转换
	for (int i = 0; i < dataBytes; i = i + 3)
	{
		rgb_struct.b = pixelArray[i + 0];
		rgb_struct.g = pixelArray[i + 1];
		rgb_struct.r = pixelArray[i + 2];

		hsv_struct = userClass_rgb_hsv.RGB2HSV(rgb_struct);			//单像素RGB转HSV

		hsv_struct.h = (int)hsv_struct.h * hScale;					//滚动条的意义在于对原hsv进行缩放
		hsv_struct.s = (float)hsv_struct.s * sScale;
		hsv_struct.v = (float)hsv_struct.v * vScale;

		rgb_struct = userClass_rgb_hsv.HSV2RGB(hsv_struct);         //将缩放后的hsv重新转换成rgb

		pixelArray[i + 0] = rgb_struct.b;							//将rgb数据还原
		pixelArray[i + 1] = rgb_struct.g;
		pixelArray[i + 2] = rgb_struct.r;

	}

	//显示图像2	
	CWnd* pWnd = GetDlgItem(IDC_STATIC_PICTURE2);					//获得pictrue控件窗口的句柄	
	
	CRect rect;
	pWnd->GetClientRect(&rect);										//获得pictrue控件所在的矩形区域			
	CDC* pDC = pWnd->GetDC();										//获得pictrue控件的DC			
	pDC->SetStretchBltMode(COLORONCOLOR);

	BITMAPINFO* pBmpInfo = (BITMAPINFO*)new char[sizeof(BITMAPINFOHEADER)];
	memcpy(pBmpInfo, &bmpdata.bmpInfo, sizeof(BITMAPINFOHEADER)); 
	StretchDIBits(pDC->GetSafeHdc(), 0, 0, rect.Width(), rect.Height(), 0, 0, bmpdata.bmpInfo.biWidth, bmpdata.bmpInfo.biHeight, pixelArray, pBmpInfo, DIB_RGB_COLORS, SRCCOPY);

	delete []pixelArray;
	return LRESULT();
}


//内部函数实现
void Cal_HSV_Scale(HSV_SLIDER_STRUCT hsv_slider_struct, float* hScale, float* sScale, float* vScale)
{
	// guangjie2333的设计

	*hScale = (float)(hsv_slider_struct.H_slider - 50) / 50 + 1; //50作为基准，小于50就按比例缩小，大于50按比例放大
	*sScale = (float)(hsv_slider_struct.S_slider - 50) / 50 + 1;
	*vScale = (float)(hsv_slider_struct.V_slider - 50) / 50 + 1;
}

void CMFCApplication1Dlg::OnBnClickedButtonOpenbmp2()
{
	// TODO: 在此添加控件通知处理程序代码

	//打开文件 
	CString filter = (CString)"图像文件(*.bmp)|*.bmp;*.BMP||";//指明可供选择的文件类型和相应的扩展名
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter, NULL);  //打开文件

//按下确定按钮 dlg.DoModal() 函数显示对话框
	if (dlg.DoModal() == IDOK)
	{
		//打开对话框获取图像信息
		BmpName = dlg.GetPathName();     //获取文件路径名   
		EntName = dlg.GetFileExt();      //获取文件扩展名
		EntName.MakeLower();                     //将文件扩展名转换为一个小写字符

		if (EntName.Compare(_T("bmp")) == 0)  //如果是bmp图片则打开显示
		{
			//定义变量存储图片信息
			BITMAPINFO* pBmpInfo;       //记录图像信息头内容


				//以只读的方式打开文件 读取bmp图片各部分 bmp文件头 信息 数据
			if (!bmpdata.bmpFile.Open(BmpName, CFile::modeRead | CFile::typeBinary))
				return;
			if (bmpdata.bmpFile.Read(&bmpdata.bmpHeader, sizeof(BITMAPFILEHEADER)) != sizeof(BITMAPFILEHEADER))
				return;
			if (bmpdata.bmpFile.Read(&bmpdata.bmpInfo, sizeof(BITMAPINFOHEADER)) != sizeof(BITMAPINFOHEADER))
				return;
			pBmpInfo = (BITMAPINFO*)new char[sizeof(BITMAPINFOHEADER)];
			//为图像数据申请空间
			memcpy(pBmpInfo, &bmpdata.bmpInfo, sizeof(BITMAPINFOHEADER));  //存储图像信息头内容
			DWORD dataBytes = bmpdata.bmpHeader.bfSize - bmpdata.bmpHeader.bfOffBits;//图像数据大小，单位为字节
			bmpdata.pBmpData = (BYTE*)new char[dataBytes];
			bmpdata.bmpFile.Seek(bmpdata.bmpHeader.bfOffBits, 0);//这一步非常重要，必须要把文件指针偏移
			bmpdata.bmpFile.Read(bmpdata.pBmpData, dataBytes);  //存储图像数据(以文件指针为起点开始读dataBytes个数据)
			bmpdata.bmpFile.Close();


			////显示图像2	
			CWnd* pWnd = GetDlgItem(IDC_STATIC_PICTURE); //获得pictrue控件窗口的句柄			
			CRect rect;
			CDC* pDC = pWnd->GetDC(); //获得pictrue控件的DC	
			pWnd = GetDlgItem(IDC_STATIC_PICTURE2); //获得pictrue控件窗口的句柄			
			pWnd->GetClientRect(&rect); //获得pictrue控件所在的矩形区域			
			pDC = pWnd->GetDC(); //获得pictrue控件的DC			
			pDC->SetStretchBltMode(COLORONCOLOR);
			StretchDIBits(pDC->GetSafeHdc(), 0, 0, rect.Width(), rect.Height(), 0, 0, bmpdata.bmpInfo.biWidth, bmpdata.bmpInfo.biHeight, bmpdata.pBmpData, pBmpInfo, DIB_RGB_COLORS, SRCCOPY);

			//打印信息
			TRACE(" rect.Width() = %d , rect.Height() = %d, bmpInfo.biWidth = %d ,  bmpInfo.biHeight = %d \n\n", rect.Width(), rect.Height(), bmpdata.bmpInfo.biWidth, bmpdata.bmpInfo.biHeight);


		}
	}
}


void CMFCApplication1Dlg::OnBnClickedButtonCompare()
{
	// TODO: 在此添加控件通知处理程序代码
	COMPARE_PICTURE userClass_Cp;
	int res;

	CWnd* pWnd1 = GetDlgItem(IDC_STATIC_PICTURE); //获得pictrue控件窗口的句柄			
	CRect rect1;
	pWnd1->GetClientRect(&rect1);
	CDC* pDC1 = pWnd1->GetDC(); //获得pictrue控件的DC

	CWnd* pWnd2 = GetDlgItem(IDC_STATIC_PICTURE2); //获得pictrue控件窗口的句柄			
	CRect rect2;
	pWnd2->GetClientRect(&rect2);
	CDC* pDC2 = pWnd2->GetDC(); //获得pictrue控件的DC

	res = userClass_Cp.ComparePicture(pDC1->GetSafeHdc(), rect1, pDC2->GetSafeHdc(), rect2) * 100;

	SetDlgItemInt(IDC_EDIT_COMPARE, res);
}

#pragma once
#include "USER_DEFINE.h"

class COMPARE_PICTURE
{
public:
	BOOL SaveHDCToFile(HDC hDC, LPRECT lpRect, LPCTSTR Dstfile);
	int ComparePicture(HDC hDC1, LPRECT lpRect1, HDC hDC2, LPRECT lpRect2);
};


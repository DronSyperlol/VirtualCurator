#pragma once
#include <gdiplus.h>  // ������������ ���� GDI+


struct WindowState
{
	HBITMAP hBmp = nullptr;
	ULONG_PTR gdiToken = 0;
};
typedef WindowState* LPWindowState;
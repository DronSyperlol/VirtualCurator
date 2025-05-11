#pragma once
#include <gdiplus.h>  // ������������ ���� GDI+


struct WindowState
{
	HBITMAP hBmp = nullptr;
	ULONG_PTR gdiToken = 0;
	Gdiplus::Bitmap* pImage = nullptr;

	bool hidden = false;
	PNOTIFYICONDATA trayData = nullptr;
};
typedef WindowState* LPWindowState;
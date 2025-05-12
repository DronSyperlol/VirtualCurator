#pragma once
#include <gdiplus.h>  // Заголовочный файл GDI+
#include <vector>

struct WindowState
{
	HBITMAP hBmp = nullptr;
	ULONG_PTR gdiToken = 0;
	Gdiplus::Bitmap* pImage = nullptr;

	bool hidden = false;
	PNOTIFYICONDATA trayData = nullptr;

	std::vector<LPWindowBase>* childs = nullptr;
};
typedef WindowState* LPWindowState;
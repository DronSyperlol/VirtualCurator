#include "WindowBase.h"

void WindowBase::initializeWindow(DWORD exStyle, LPCWSTR className, LPCWSTR windowName, DWORD style, 
	int x, int y, int width, int height, HWND parent, HMENU menu, LPVOID lp)
{
	_hWnd = CreateWindowEx(exStyle, className, windowName, style, x, y, width, height,
							parent, menu, _hInstance, lp);
	if (_hWnd == nullptr) throw "Window cannot created";
	_createdWindows[_hWnd] = this;
}

void WindowBase::show(bool toShow) const
{
	ShowWindowAsync(_hWnd, toShow ? SW_SHOW : SW_HIDE);
}

WindowBase::WindowBase(HINSTANCE hInst)
{
	_hInstance = hInst;
}

WindowBase::~WindowBase()
{
	_createdWindows.erase(_hWnd);
}

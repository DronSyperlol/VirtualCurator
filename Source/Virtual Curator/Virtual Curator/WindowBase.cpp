#include "WindowBase.h"


void WindowBase::initializeWindow(DWORD exStyle, LPCWSTR className, LPCWSTR windowName, DWORD style, 
	int x, int y, int width, int height, HWND parent, HMENU menu, LPVOID lp)
{
	_hWnd = CreateWindowEx(exStyle, className, windowName, style, x, y, width, height,
							parent, menu, nullptr, lp);
	if (_hWnd == nullptr) throw "Window cannot created";
	_createdWindows[_hWnd] = this;
}

LRESULT WindowBase::onWindowCreated(WPARAM wp, LPARAM lp) const
{
	return onRawWndProc(WM_CREATE, wp, lp);
}

LRESULT WindowBase::onRawWndProc(UINT msg, WPARAM wp, LPARAM lp) const
{
	return DefWindowProc(_hWnd, msg, wp, lp);
}

void WindowBase::show(bool toShow) const
{
	ShowWindowAsync(_hWnd, toShow ? SW_SHOW : SW_HIDE);
}

std::map<HWND, WindowBase*> WindowBase::_createdWindows;

WindowBase* WindowBase::getWindowByHWnd(HWND hWnd)
{
	return _createdWindows[hWnd];
}

LRESULT WindowBase::routeEvents(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	WindowBase* wndClass = getWindowByHWnd(hWnd);
	if (!wndClass) return DefWindowProc(hWnd, msg, wp, lp);
	switch (msg) {
	case WM_CREATE: 
		return wndClass->onWindowCreated(wp, lp);
	default:
		return wndClass->onRawWndProc(msg, wp, lp);
	}
}

WindowBase::WindowBase(HINSTANCE hInst)
{
	_hWnd = nullptr;
	_hInstance = hInst;
}

WindowBase::~WindowBase()
{
	_createdWindows.erase(_hWnd);
}

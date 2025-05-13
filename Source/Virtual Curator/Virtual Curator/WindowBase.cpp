#include "WindowBase.h"


HWND WindowBase::initializeWindow(DWORD exStyle, LPCWSTR windowName, DWORD style,
	int x, int y, int width, int height, HMENU menu)
{
	return CreateWindowEx(exStyle, getClassName(), windowName, style, x, y, width, height, _parent, menu, _hInstance, this);
}

LRESULT WindowBase::onRawWndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) const
{
	return DefWindowProc(_hWnd, msg, wp, lp);
}

LRESULT WindowBase::onCommand(HWND hWnd, WPARAM wp, LPARAM lp) const
{
	return onRawWndProc(hWnd, WM_COMMAND, wp, lp);
}

void WindowBase::show(bool toShow) const
{
	ShowWindowAsync(_hWnd, toShow ? SW_SHOW : SW_HIDE);
}

void WindowBase::move(int x, int y) const
{
	SetWindowPos(_hWnd, nullptr, x, y, 0, 0, SWP_NOSIZE);
}

void WindowBase::sendMessage(UINT msg, WPARAM wp, LPARAM lp) const
{
	SendMessage(_hWnd, msg, wp, lp);
}

void WindowBase::destroyWindow()
{
	DestroyWindow(_hWnd);
	_hWnd = 0;
	_hInstance = 0;
}

LRESULT WINAPI WindowBase::routeEvents(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	auto wndClass = (LPWindowBase)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	try {
		if (wndClass == nullptr && (msg == WM_CREATE || msg == WM_NCCREATE)) {
			wndClass = (LPWindowBase)((LPCREATESTRUCT)lp)->lpCreateParams;
			wndClass->_hWnd = hWnd;
			SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)wndClass);
		}
	}
	catch (std::exception) {}
	if (!wndClass) return DefWindowProc(hWnd, msg, wp, lp);
	switch (msg) {
	case WM_CREATE:
		return wndClass->onWindowCreate(hWnd, wp, lp);
	case WM_DESTROY:
		return wndClass->onWindowDestroy(hWnd, wp, lp);
	case WM_COMMAND:
		return wndClass->onCommand(hWnd, wp, lp);
	default:
		return wndClass->onRawWndProc(hWnd, msg, wp, lp);
	}
}

WindowBase::WindowBase(HINSTANCE hInst)
{
	_hWnd = nullptr;
	_hInstance = hInst;
}

WindowBase::~WindowBase()
{
}

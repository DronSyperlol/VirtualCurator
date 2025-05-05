#include "MainWindow.h"

WNDCLASSEX* MainWindow::_wndClass = nullptr;


LPCWSTR MainWindow::getClassName() const {
	if (_wndClass == nullptr) {
		_wndClass = new WNDCLASSEX;
		ZeroMemory(_wndClass, sizeof(WNDCLASSEX));
		_wndClass->cbSize = sizeof(WNDCLASSEX);
		_wndClass->lpfnWndProc = (WNDPROC)WindowBase::routeEvents;
		_wndClass->lpszClassName = L"MainWindow";
		_wndClass->hInstance = _hInstance;
		_wndClass->hIcon = LoadIcon(_hInstance, IDI_QUESTION);
		_wndClass->hCursor = LoadCursor(_hInstance, IDC_ARROW);
		_wndClass->hbrBackground = (HBRUSH)COLOR_WINDOW;
		//_wndClass->hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
		if (!RegisterClassEx(_wndClass)) throw "Cannot register class";
	}
	return _wndClass->lpszClassName;
}


LRESULT MainWindow::onWindowCreated(HWND hWnd, WPARAM wp, LPARAM lp) const
{
	CreateWindowEx(0, L"static", L"", WS_CHILD | WS_BORDER, 10, 10, 100, 50, hWnd, nullptr, _hInstance, nullptr);
	return DefWindowProc(hWnd, WM_CREATE, wp, lp);
}

LRESULT MainWindow::onWindowDestroyed(HWND hWnd, WPARAM wp, LPARAM lp) const
{
	PostQuitMessage(0);
	return DefWindowProc(hWnd, WM_DESTROY, wp, lp);
}

LRESULT MainWindow::onRawWndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) const
{
	switch (msg)
	{
	case WM_LBUTTONDOWN:
		SendMessage(hWnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
		break;
	default:
		break;
	}
	return DefWindowProc(hWnd, msg, wp, lp);
}

MainWindow::MainWindow(HINSTANCE hInst) : WindowBase(hInst)
{
	initializeWindow(WS_EX_TOPMOST, L"Main Windows", WS_POPUP, 100, 100, 300, 300, NULL, NULL);
	show(true);
}

MainWindow::~MainWindow() 
{ }

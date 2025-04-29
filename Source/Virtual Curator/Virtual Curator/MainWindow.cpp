#include "MainWindow.h"


LRESULT wndProc(HWND hWnd, UINT uInt, WPARAM wp, LPARAM lp) {
	return 0;
}

WNDCLASS* MainWindow::_wndClass = nullptr;

LPCWSTR MainWindow::getClassName() const {
	if (_wndClass == nullptr) {
		_wndClass = new WNDCLASS;
		ZeroMemory(_wndClass, sizeof(WNDCLASS));
		_wndClass->style = NULL;
		_wndClass->lpfnWndProc = wndProc;
		_wndClass->lpszClassName = L"MainWindow";
		_wndClass->hInstance = _hInstance;
		_wndClass->hIcon = LoadIcon(_hInstance, IDI_QUESTION);
		_wndClass->hCursor = LoadCursor(_hInstance, IDC_ARROW);
		_wndClass->hbrBackground = (HBRUSH)COLOR_WINDOW;
		if (!RegisterClass(_wndClass)) throw "Cannot register class";
	}
	return _wndClass->lpszClassName;
}

MainWindow::MainWindow(HINSTANCE hInst) : WindowBase(hInst)
{
	initializeWindow(NULL, getClassName(), L"Main Windows", WS_OVERLAPPEDWINDOW, 100, 100, 300, 300, NULL, NULL, NULL);
	//show(true);
}

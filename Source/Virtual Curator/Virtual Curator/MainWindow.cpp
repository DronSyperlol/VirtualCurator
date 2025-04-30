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
		if (!RegisterClassEx(_wndClass)) throw "Cannot register class";
	}
	return _wndClass->lpszClassName;
}

LRESULT MainWindow::onWindowCreated(WPARAM wp, LPARAM lp) const
{
	MessageBox(_hWnd, L"Окно отрисовано!", L"Success", MB_OK);
	return WindowBase::onWindowCreated(wp, lp);
}

MainWindow::MainWindow(HINSTANCE hInst) : WindowBase(hInst)
{
	initializeWindow(NULL, getClassName(), L"Main Windows", WS_OVERLAPPEDWINDOW, 100, 100, 300, 300, NULL, NULL, NULL);
	show(true);
}

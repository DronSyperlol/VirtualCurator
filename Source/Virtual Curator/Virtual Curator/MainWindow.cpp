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


void onBtnClick() {
	MessageBox(nullptr, L"Clicked", L"Btn test", MB_OK);
}

LRESULT MainWindow::onWindowCreated(HWND hWnd, WPARAM wp, LPARAM lp) const
{
	auto btn = new VirtualCurator::Button(_hInstance, hWnd, L"Push me", 10, 10, 100, 50, onBtnClick);
	return DefWindowProc(hWnd, WM_CREATE, wp, lp);
}

LRESULT MainWindow::onWindowDestroyed(HWND hWnd, WPARAM wp, LPARAM lp) const
{
	PostQuitMessage(0);
	return DefWindowProc(hWnd, WM_DESTROY, wp, lp);
}

LRESULT MainWindow::onRawWndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) const
{
	if (msg == WM_COMMAND) {
		MessageBox(hWnd, L"Command", L"cmd", MB_OK);
	}
	return DefWindowProc(hWnd, msg, wp, lp);
	return LRESULT();
}

MainWindow::MainWindow(HINSTANCE hInst) : WindowBase(hInst)
{
	initializeWindow(NULL, L"Main Windows", WS_OVERLAPPEDWINDOW, 100, 100, 300, 300, NULL, NULL);
	show(true);
}

#include "MainWindow.h"
#pragma comment(lib, "gdiplus.lib")  // Автоматическая линковка библиотеки
#include "Tools.h"

WNDCLASSEX* MainWindow::_wndClass = nullptr;


LPCWSTR MainWindow::getClassName() const {
	if (_wndClass == nullptr) {
		_wndClass = new WNDCLASSEX;
		ZeroMemory(_wndClass, sizeof(WNDCLASSEX));
		_wndClass->cbSize = sizeof(WNDCLASSEX);
		_wndClass->style = 0;
		_wndClass->lpfnWndProc = (WNDPROC)WindowBase::routeEvents;
		_wndClass->lpszClassName = L"MainWindow";
		_wndClass->hInstance = _hInstance;
		if (!RegisterClassEx(_wndClass)) throw "Cannot register class";
	}
	return _wndClass->lpszClassName;
}


LRESULT MainWindow::onWindowCreated(HWND hWnd, WPARAM wp, LPARAM lp) const
{
	Gdiplus::GdiplusStartupInput gdiInput;
	Gdiplus::GdiplusStartup(&_wndState->gdiToken, &gdiInput, NULL);

	LPCWSTR imageName = L"..\\..\\..\\Data\\Images\\default-maskot.png";
	_wndState->pImage = new Gdiplus::Bitmap(imageName);
	if (_wndState->pImage != nullptr && _wndState->pImage->GetLastStatus() != Gdiplus::Ok) {
		delete_ptr(_wndState->pImage);
	}
	else {
		_wndState->pImage->GetHBITMAP(Gdiplus::Color(0, 0, 0, 0), &_wndState->hBmp);
		drawWindow();
	}
	return DefWindowProc(hWnd, WM_CREATE, wp, lp);
}

LRESULT MainWindow::onWindowDestroyed(HWND hWnd, WPARAM wp, LPARAM lp) const
{
	Gdiplus::GdiplusShutdown(_wndState->gdiToken);
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

void MainWindow::drawWindow() const
{
	const int zoom = 0;
	RECT rect = { 0 };
	GetWindowRect(_hWnd, &rect);

	BITMAP bm = { 0 };
	GetObject(_wndState->hBmp, sizeof(BITMAP), &bm);


	SIZE size = { bm.bmWidth, bm.bmHeight };


	HDC hdcMem = CreateCompatibleDC(NULL);
	HBITMAP hOld = (HBITMAP)SelectObject(hdcMem, _wndState->hBmp);


	BLENDFUNCTION blend = { 0 };
	blend.AlphaFormat = AC_SRC_ALPHA;
	blend.BlendOp = AC_SRC_OVER;
	blend.SourceConstantAlpha = 255;

	POINT ptZero = { 0 };

	POINT ptWindow = { 0 };
	ClientToScreen(_hWnd, &ptWindow);

	UpdateLayeredWindow(_hWnd, NULL, &ptWindow, &size, hdcMem, &ptZero, RGB(0, 0, 0), &blend, ULW_COLORKEY);

	DeleteDC(hdcMem);
}

MainWindow::MainWindow(HINSTANCE hInst) : WindowBase(hInst)
{
	_wndState = new WindowState;
	initializeWindow(WS_EX_TOPMOST | WS_EX_LAYERED, L"Main Windows", WS_POPUP, 100, 100, 900, 900, NULL, NULL);
	show(true);
}

MainWindow::~MainWindow()
{
	delete_ptr(_wndState->hBmp);
	delete_ptr(_wndState->pImage);
	delete_ptr(_wndState);
}

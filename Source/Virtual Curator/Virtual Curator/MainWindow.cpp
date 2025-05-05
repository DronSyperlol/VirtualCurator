#include "MainWindow.h"
#pragma comment(lib, "gdiplus.lib")  // Автоматическая линковка библиотеки

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
		_wndClass->hIcon = LoadIcon(_hInstance, IDI_QUESTION);
		_wndClass->hCursor = LoadCursor(_hInstance, IDC_ARROW);
		//_wndClass->hbrBackground = (HBRUSH)COLOR_WINDOW;
		_wndClass->hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH); // кривая прозрачность
		if (!RegisterClassEx(_wndClass)) throw "Cannot register class";
	}
	return _wndClass->lpszClassName;
}


LRESULT MainWindow::onWindowCreated(HWND hWnd, WPARAM wp, LPARAM lp) const
{
	LPCWSTR imageName = L"D:\\GitHub\\Maskot-Notifier\\Data\\Images\\default-maskot1.bmp";
	_wndState->hBmp = (HBITMAP)LoadImage(NULL, imageName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	Gdiplus::GdiplusStartupInput gdiInput;
	Gdiplus::GdiplusStartup(&_wndState->gdiToken, &gdiInput, NULL);

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
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		LPCWSTR imageName = L"..\\..\\..\\Data\\Images\\default-maskot.png";
		Gdiplus::Bitmap bitmap(imageName);
		if (bitmap.GetLastStatus() == Gdiplus::Ok) {
			Gdiplus::Graphics graphics(hdc);
			RECT rect = { 0 };
			GetWindowRect(hWnd, &rect);
			const int zoom = 60;
			graphics.DrawImage(&bitmap, 0 - zoom/2, 0 - zoom/2, rect.right - rect.left + zoom, rect.bottom - rect.top + zoom);
		}

		EndPaint(hWnd, &ps);
	}
	break;
	default:
		break;
	}
	return DefWindowProc(hWnd, msg, wp, lp);
}

MainWindow::MainWindow(HINSTANCE hInst) : WindowBase(hInst)
{
	_wndState = new WindowState;
	initializeWindow(WS_EX_TOPMOST, L"Main Windows", WS_POPUP, 100, 100, 500, 500, NULL, NULL);
	show(true);
}

MainWindow::~MainWindow()
{
	delete _wndState;
	_wndState = nullptr;
}

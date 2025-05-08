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
	Gdiplus::GdiplusStartupInput gdiInput;
	Gdiplus::GdiplusStartup(&_wndState->gdiToken, &gdiInput, NULL);

	LPCWSTR imageName = L"..\\..\\..\\Data\\Images\\default-maskot.png";
	_wndState->pImage = new Gdiplus::Bitmap(imageName);
	if (_wndState->pImage != nullptr && _wndState->pImage->GetLastStatus() != Gdiplus::Ok) {
		delete_ptr(_wndState->pImage);
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
		SendMessage(hWnd, WM_PAINT, NULL, NULL);
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		if (_wndState->pImage) {
			Gdiplus::Graphics graphics(hdc);
			RECT rect = { 0 };
			GetWindowRect(hWnd, &rect);
			const int zoom = 60;
			Gdiplus::Color color(0, 0, 0, 0);
			graphics.Clear(color);
			graphics.DrawImage(_wndState->pImage, 0 - zoom / 2, 0 - zoom / 2, rect.right - rect.left + zoom, rect.bottom - rect.top + zoom);
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
	delete_ptr(_wndState->hBmp);
	delete_ptr(_wndState->pImage);
	delete_ptr(_wndState);
}

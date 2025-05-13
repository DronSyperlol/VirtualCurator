#include "NotifyWindow.h"
#include "gdiplus.h"
#pragma comment(lib, "gdiplus.lib")

WNDCLASSEX* NotifyWindow::_wndClass = nullptr;

LPCWSTR NotifyWindow::getClassName() const
{
	if (_wndClass == nullptr)
	{
		_wndClass = new WNDCLASSEX;
		ZeroMemory(_wndClass, sizeof(WNDCLASSEX));
		_wndClass->cbSize = sizeof(WNDCLASSEX);
		_wndClass->lpfnWndProc = (WNDPROC)WindowBase::routeEvents;
		_wndClass->hInstance = _hInstance;
		_wndClass->lpszClassName = L"MessageNotify";
		if (!RegisterClassEx(_wndClass)) throw "Cannot register class";
	}
	return _wndClass->lpszClassName;
}

LRESULT NotifyWindow::onWindowCreate(HWND hWnd, WPARAM wp, LPARAM lp) const
{
	Gdiplus::GdiplusStartupInput gdiInput;
	Gdiplus::GdiplusStartup(&_wndState->gdiToken, &gdiInput, NULL);

	LPCWSTR imageName = L"..\\..\\..\\Data\\Images\\message.png";
	_wndState->pImage = new Gdiplus::Bitmap(imageName);
	if (_wndState->pImage != nullptr && _wndState->pImage->GetLastStatus() != Gdiplus::Ok) {
		delete _wndState->pImage;
	}
	else {
		_wndState->pImage->GetHBITMAP(Gdiplus::Color(0, 0, 0, 0), &_wndState->hBmp);
		drawWindow();
		SetCursor(LoadCursor(NULL, IDC_ARROW));
	}
	
	return DefWindowProc(hWnd, WM_CREATE, wp, lp);
}

LRESULT NotifyWindow::onWindowDestroy(HWND hWnd, WPARAM wp, LPARAM lp) const
{
	Gdiplus::GdiplusShutdown(_wndState->gdiToken);
	return DefWindowProc(hWnd, WM_DESTROY, wp, lp);
}


void NotifyWindow::drawWindow() const
{
	RECT rect = { 0 };
	GetWindowRect(_hWnd, &rect);
	SIZE wndSize = { rect.right - rect.left, rect.bottom - rect.top };
	POINT wndPos = { 0 };
	ClientToScreen(_hWnd, &wndPos);

	BITMAP bm = { 0 };
	GetObject(_wndState->hBmp, sizeof(BITMAP), &bm);

	//
	wndSize.cy = wndSize.cx / ((double)bm.bmWidth / bm.bmHeight);
	wndPos.y -= wndSize.cy;
	if (wndPos.y < 0) wndPos.y = 0;

	HDC srcImageDC = CreateCompatibleDC(NULL);
	SelectObject(srcImageDC, _wndState->hBmp);

	HBITMAP dstBmp = CreateCompatibleBitmap(srcImageDC, wndSize.cx, wndSize.cy);
	HDC dstImageDC = CreateCompatibleDC(NULL);
	SelectObject(dstImageDC, dstBmp);

	SetStretchBltMode(dstImageDC, HALFTONE);
	StretchBlt(dstImageDC, 0, 0, wndSize.cx, wndSize.cy, srcImageDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

	Gdiplus::Graphics graphics(dstImageDC);

	Gdiplus::Font myFont(L"windings", 16);

	const int padding = 20;

	Gdiplus::RectF therect;
	therect.Height = wndSize.cy - padding * 2;
	therect.Width = wndSize.cx - padding*2;
	therect.Y = therect.X = padding;

	Gdiplus::StringFormat format;
	format.SetAlignment(Gdiplus::StringAlignmentNear);
	format.GenericDefault();

	Gdiplus::SolidBrush   GxTextBrush(Gdiplus::Color(255, 10, 10, 10));

	int msglen = 0;
	while (_message[msglen] != '\0') msglen++;

	graphics.DrawString(_message, msglen, &myFont, therect, &format, &GxTextBrush);

	BLENDFUNCTION blend = { 0 };
	blend.AlphaFormat = AC_SRC_ALPHA;
	blend.BlendOp = AC_SRC_OVER;
	blend.SourceConstantAlpha = 255;

	POINT ptZero = { 0 };

	UpdateLayeredWindow(_hWnd, NULL, &wndPos, &wndSize, dstImageDC, &ptZero, RGB(0, 0, 0), &blend, ULW_COLORKEY);

	DeleteDC(srcImageDC);
	DeleteObject(dstBmp);
	DeleteDC(dstImageDC);
	graphics.ReleaseHDC(dstImageDC);
}

NotifyWindow::NotifyWindow(HINSTANCE hInst, HWND parent, LPCWSTR message) : WindowBase(hInst)
{
	_message = message;
	_wndState = new WindowState;
	RECT parentWndRect = { 0 };
	GetWindowRect(parent, &parentWndRect);
	initializeWindow(WS_EX_TOPMOST | WS_EX_LAYERED,
		L"Message",
		WS_POPUP,
		parentWndRect.left, parentWndRect.top - 100,
		parentWndRect.right - parentWndRect.left, 0,
		parent, NULL);
	show(true);
}

NotifyWindow::~NotifyWindow()
{
	delete _wndState->pImage;
	delete _wndState->trayData;
	delete _wndState->childs;
	delete _wndState;
}

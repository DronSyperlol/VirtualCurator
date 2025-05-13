#include "MainWindow.h"
#pragma comment(lib, "gdiplus.lib") 
#include "resource.h"
#include "NotifyWindow.h"
#include <thread>
#include <chrono>


WNDCLASSEX* MainWindow::_wndClass = nullptr;


void MainWindow::lifeTimerProc(HWND hWnd, UINT msg, UINT_PTR ptr, DWORD dword)
{
	auto ownerWnd = (MainWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	auto wndToKill = (LPWindowBase)ptr;
	KillTimer(hWnd, ptr);
	ownerWnd->killChild(wndToKill);
}

LPCWSTR MainWindow::getClassName() const {
	if (_wndClass == nullptr) {
		_wndClass = new WNDCLASSEX;
		ZeroMemory(_wndClass, sizeof(WNDCLASSEX));
		_wndClass->cbSize = sizeof(WNDCLASSEX);
		_wndClass->style = 0;
		_wndClass->lpfnWndProc = (WNDPROC)WindowBase::routeEvents;
		_wndClass->lpszClassName = L"MainWindow";
		_wndClass->hInstance = _hInstance;
		_wndClass->hIcon = LoadIcon(_hInstance, MAKEINTRESOURCE(IDI_ICON1));
		if (!RegisterClassEx(_wndClass)) throw "Cannot register class";
	}
	return _wndClass->lpszClassName;
}

LRESULT MainWindow::onWindowCreate(HWND hWnd, WPARAM wp, LPARAM lp) const
{
	Gdiplus::GdiplusStartupInput gdiInput;
	Gdiplus::GdiplusStartup(&_wndState->gdiToken, &gdiInput, NULL);

	LPCWSTR imageName = L"..\\..\\..\\Data\\Images\\default-maskot.png";
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

LRESULT MainWindow::onWindowDestroy(HWND hWnd, WPARAM wp, LPARAM lp) const
{
	Gdiplus::GdiplusShutdown(_wndState->gdiToken);
	PostQuitMessage(0);
	return DefWindowProc(hWnd, WM_DESTROY, wp, lp);
}

LRESULT MainWindow::onCommand(HWND hWnd, WPARAM wp, LPARAM lp) const
{
	switch (LOWORD(wp)) {
	default:
		break;
	}
	return WindowBase::onCommand(hWnd, wp, lp);
}

LRESULT MainWindow::onRawWndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) const
{
	switch (msg)
	{
	case WM_LBUTTONDOWN:
		SendMessage(hWnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
		break;
	case WM_MOVE:
	{
		POINT currentPos = { 0 };
		ClientToScreen(hWnd, &currentPos);
		for (auto child : *(_wndState->childs)) {
			NotifyWindow* childWnd = dynamic_cast<NotifyWindow*>(child);
			if (childWnd) {
				childWnd->move(currentPos.x, currentPos.y);
			}
		}
	}
	break;
	case WM_CONTEXTMENU:
	{
		int x = LOWORD(lp);
		int y = HIWORD(lp);
		drawContextMenu(x, y);
	}
	break;
	case WM_USER:
	{
		switch (lp)
		{
		case WM_LBUTTONDOWN:
			showFromTray();
			break;
		case MASKOT_SAY:
			notifyAsMessage((LPCWSTR)wp);
			break;
		}
	}
	break;
	default:
		break;
	}
	return DefWindowProc(hWnd, msg, wp, lp);
}

void MainWindow::drawWindow() const
{
	RECT rect = { 0 };
	GetWindowRect(_hWnd, &rect);
	SIZE wndSize = { rect.right - rect.left, rect.bottom - rect.top };
	POINT wndPos = { 0 };
	ClientToScreen(_hWnd, &wndPos);

	BITMAP bm = { 0 };
	GetObject(_wndState->hBmp, sizeof(BITMAP), &bm);

	HDC srcImageDC = CreateCompatibleDC(NULL);
	SelectObject(srcImageDC, _wndState->hBmp);

	HBITMAP dstBmp = CreateCompatibleBitmap(srcImageDC, wndSize.cx, wndSize.cy);
	HDC dstImageDC = CreateCompatibleDC(NULL);
	SelectObject(dstImageDC, dstBmp);

	SetStretchBltMode(dstImageDC, HALFTONE);
	StretchBlt(dstImageDC, 0, 0, wndSize.cx, wndSize.cy, srcImageDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

	BLENDFUNCTION blend = { 0 };
	blend.AlphaFormat = AC_SRC_ALPHA;
	blend.BlendOp = AC_SRC_OVER;
	blend.SourceConstantAlpha = 255;

	POINT ptZero = { 0 };

	UpdateLayeredWindow(_hWnd, NULL, &wndPos, &wndSize, dstImageDC, &ptZero, RGB(0, 0, 0), &blend, ULW_COLORKEY);

	DeleteDC(srcImageDC);
	DeleteObject(dstBmp);
	DeleteDC(dstImageDC);
}

void MainWindow::drawContextMenu(int x, int y) const
{
	HMENU rootMenu = CreatePopupMenu();
	AppendMenu(rootMenu, MF_STRING, CMI_HIDE, L"Скрыть");
	int code = TrackPopupMenu(rootMenu, TPM_LEFTALIGN | TPM_BOTTOMALIGN | TPM_RETURNCMD | TPM_LEFTBUTTON | TPM_NOANIMATION, x, y, 0, _hWnd, nullptr);
	processContextMenu(code);
}

void MainWindow::processContextMenu(int code) const
{
	switch (code)
	{
	case CMI_HIDE:
		hideToTray();
		break;
	default:
		break;
	}
}

void MainWindow::hideToTray() const
{
	if (_wndState->hidden) return;
	if (!_wndState->trayData) {
		_wndState->trayData = new NOTIFYICONDATA;
		ZeroMemory(_wndState->trayData, sizeof(NOTIFYICONDATA));
		_wndState->trayData->cbSize = sizeof(NOTIFYICONDATA);
		_wndState->trayData->hWnd = _hWnd;
		_wndState->trayData->uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE | NIF_SHOWTIP;
		_wndState->trayData->hIcon = LoadIcon(_hInstance, MAKEINTRESOURCE(IDI_ICON1));
		WCHAR tip[] = L"Показать куратора";
		wcsncpy_s(_wndState->trayData->szTip, tip, sizeof(tip));
		_wndState->trayData->uCallbackMessage = WM_USER;
		_wndState->trayData->uVersion = NOTIFYICON_VERSION_4;
	}
	Shell_NotifyIcon(NIM_ADD, _wndState->trayData);
	Shell_NotifyIcon(NIM_SETVERSION, _wndState->trayData);
	show(false);
	_wndState->hidden = true;
}

void MainWindow::showFromTray() const
{
	if (!_wndState->hidden || _wndState->trayData == nullptr) return;
	Shell_NotifyIcon(NIM_DELETE, _wndState->trayData);
	show(true);
	_wndState->hidden = false;
}

void MainWindow::notifyAsMessage(LPCWSTR notificationMessage) const
{
	if (_wndState->hidden) showFromTray();
	LPWindowBase child = new NotifyWindow(_hInstance, _hWnd, notificationMessage);
	_wndState->childs->push_back(child);
	SetTimer(_hWnd, (UINT_PTR)child, NOTIFICATION_SHOW_SEC * 1000, lifeTimerProc);
}

MainWindow::MainWindow(HINSTANCE hInst) : WindowBase(hInst)
{
	_wndState = new WindowState;
	ZeroMemory(_wndState, sizeof(WindowState));
	_wndState->childs = new std::vector<LPWindowBase>();
	initializeWindow(WS_EX_TOPMOST | WS_EX_LAYERED, L"Виртуальный куратор", WS_POPUP, 100, 100, 300, 300, NULL, NULL);
	show(true);
}

MainWindow::~MainWindow()
{
	DeleteObject(_wndState->hBmp);
	delete _wndState->pImage;
	delete _wndState->trayData;
	delete _wndState->childs;
	delete _wndState;
}

void MainWindow::killChild(LPWindowBase childToKill) const
{
	childToKill->destroyWindow();
	delete childToKill;
	auto v = _wndState->childs;
	v->erase(std::find(v->begin(), v->end(), childToKill));
	childToKill = nullptr;
}

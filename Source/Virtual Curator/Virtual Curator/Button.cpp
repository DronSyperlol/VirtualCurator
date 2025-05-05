#include "Button.h"

VirtualCurator::Button::Button(HINSTANCE hInst, HWND parent, LPCWSTR buttonName, int x, int y, int width, int height, void(*onClick)()) : WindowBase(hInst)
{
	_onClick = onClick;
	_hWnd = initializeWindow(0, buttonName, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, x, y, width, height, parent, nullptr);
	SetWindowLongPtr(_hWnd, GWLP_USERDATA, (LONG_PTR)this);
	show(true);
}


LPCWSTR VirtualCurator::Button::getClassName() const
{
	return L"button";
}

LRESULT VirtualCurator::Button::onWindowCreated(HWND hWnd, WPARAM wp, LPARAM lp) const
{
	return DefWindowProc(hWnd, WM_CREATE, wp, lp);
}

LRESULT VirtualCurator::Button::onWindowDestroyed(HWND hWnd, WPARAM wp, LPARAM lp) const
{
	return DefWindowProc(hWnd, WM_DESTROY, wp, lp);
}

LRESULT VirtualCurator::Button::onRawWndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) const
{
	switch (msg)
	{
	case WM_COMMAND:
		_onClick();
		break;
	}
	return DefWindowProc(hWnd, WM_DESTROY, wp, lp);
}

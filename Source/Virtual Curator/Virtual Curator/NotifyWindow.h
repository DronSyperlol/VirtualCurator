#pragma once
#include "WindowBase.h"
#include "WindowState.h"

class NotifyWindow : public WindowBase {
private: 
	static WNDCLASSEX* _wndClass;
	LPCWSTR getClassName() const override;

	LRESULT onWindowCreate(HWND, WPARAM, LPARAM) const override;
	LRESULT onWindowDestroy(HWND, WPARAM, LPARAM) const override;

	LPWindowState _wndState = nullptr;
	LPCWSTR _message = nullptr;

	void drawWindow() const;

public:
	NotifyWindow(HINSTANCE hInst, HWND parent, LPCWSTR message);
	~NotifyWindow();
};
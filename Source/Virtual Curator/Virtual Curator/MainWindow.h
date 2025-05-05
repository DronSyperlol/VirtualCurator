#pragma once
#include "WindowBase.h"
#include "WindowState.h"

class MainWindow : public WindowBase {
private:
	static WNDCLASSEX* _wndClass;
	LPCWSTR getClassName() const override;
	LRESULT onWindowCreated(HWND, WPARAM, LPARAM) const override;
	LRESULT onWindowDestroyed(HWND, WPARAM, LPARAM) const override;
	LRESULT onRawWndProc(HWND, UINT, WPARAM, LPARAM) const override;

	LPWindowState _wndState;

public: 
	MainWindow(HINSTANCE);
	~MainWindow();
};
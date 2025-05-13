#pragma once
#include "WindowBase.h"
#include "WindowState.h"
#include <vector>

// Menu
const int CMI_HIDE = 1;

// Tracking
const int MASKOT_SAY = 100;




class MainWindow : public WindowBase {
private:
	static WNDCLASSEX* _wndClass;
	LPCWSTR getClassName() const override;
	LRESULT onWindowCreate(HWND, WPARAM, LPARAM) const override;
	LRESULT onWindowDestroy(HWND, WPARAM, LPARAM) const override;
	LRESULT onCommand(HWND, WPARAM, LPARAM) const override;
	LRESULT onRawWndProc(HWND, UINT, WPARAM, LPARAM) const override;

	LPWindowState _wndState;

	void drawWindow() const;
	void drawContextMenu(int x, int y) const;

	void processContextMenu(int) const;

	void hideToTray() const;
	void showFromTray() const;

public: 
	MainWindow(HINSTANCE);
	~MainWindow();
};
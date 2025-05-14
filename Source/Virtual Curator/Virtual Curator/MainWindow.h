#pragma once
#include "WindowBase.h"
#include "WindowState.h"


// Menu
const int CMI_HIDE = 1;
const int CMI_AUTHORS = 2;

// Tracking
const int MASKOT_SAY = 100;
const int MASKOT_ENDSAY = 101;


// Consts
const int NOTIFICATION_SHOW_SEC = 10;

class MainWindow : public WindowBase {
private:
	static WNDCLASSEX* _wndClass;
	static void lifeTimerProc(HWND, UINT, UINT_PTR, DWORD);

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

	void notifyAsMessage(LPCWSTR notificationMessage) const;

public: 
	MainWindow(HINSTANCE);
	~MainWindow();

protected:
	void killChild(LPWindowBase) const;
};
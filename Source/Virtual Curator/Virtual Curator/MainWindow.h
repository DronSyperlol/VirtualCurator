#pragma once
#include "WindowBase.h"

class MainWindow : public WindowBase {
private:
	static WNDCLASSEX* _wndClass;
	LPCWSTR getClassName() const override;
	LRESULT onWindowCreated(WPARAM, LPARAM) const override;
public: 
	MainWindow(HINSTANCE);
};
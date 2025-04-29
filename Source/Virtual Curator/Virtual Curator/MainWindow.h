#pragma once
#include "WindowBase.h"
class MainWindow : public WindowBase {
private:
	static WNDCLASS* _wndClass;
	LPCWSTR getClassName() const override;


public: 
	MainWindow(HINSTANCE);

};
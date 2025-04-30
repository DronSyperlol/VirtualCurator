#include "MainWindow.h"	

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR cmdParam, int cmdParamCount) {

	MainWindow mainWnd(hInstance);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) != 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}
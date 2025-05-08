#include "main.h"
#include "Tools.h"

using namespace VirtualCurator;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR cmdParam, int cmdParamCount) 
{
	auto mainWindow = new MainWindow(hInst);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) != 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	// closing app
	delete_ptr(mainWindow);
}

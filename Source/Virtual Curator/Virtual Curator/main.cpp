#include "main.h"
#include <thread>
#include "TrackProcesses.h"
#include "TrackedWindow.h"

const int MAX_ADDITION_THREADS = 2;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR cmdParam, int cmdParamCount) 
{
	auto mainWindow = new MainWindow(hInst);

	std::thread trackingThread(TrackForeground, mainWindow);
	trackingThread.detach();
	
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) != 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	// closing app
	mainWindow->destroyWindow();
	delete mainWindow;
}

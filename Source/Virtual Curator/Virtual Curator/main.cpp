#include "main.h"
#include <thread_pool/thread_pool.h>
#include "TrackProcesses.h"
#include "TrackedWindow.h"

const int MAX_ADDITION_THREADS = 2;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR cmdParam, int cmdParamCount) 
{
	auto mainWindow = new MainWindow(hInst);

	dp::thread_pool threads(MAX_ADDITION_THREADS);

	threads.enqueue_detach(TrackForeground, mainWindow);


	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) != 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	// closing app
	delete mainWindow;
}

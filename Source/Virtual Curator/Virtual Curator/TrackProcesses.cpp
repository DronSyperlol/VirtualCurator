#include "TrackProcesses.h"
#include <chrono>
#include <thread>
#include <map>
#include "TrackedWindow.h"
#include "TrackTarget.h"

static std::map<HWND, LPTrackedWindow> cache;
static std::vector<std::wstring> targets
{
	L"Youtube",
	L"TikTok"
};

void TrackProcesses()
{
	const int buffer_size = 1024;
	while (true) {

		HWND wnd = GetForegroundWindow();
		auto elem = cache[wnd];
		if (elem == nullptr) {
			elem = new TrackedWindow;
			elem->hWnd = wnd;
			elem->startTracking = std::chrono::steady_clock::now();
			elem->title = new WCHAR[buffer_size];
			GetWindowText(wnd, elem->title, buffer_size);
		}
		else {

		}
		std::this_thread::sleep_for(std::chrono::seconds(10));
	}
}

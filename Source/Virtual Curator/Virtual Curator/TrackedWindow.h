#pragma once
#include <Windows.h>
#include <chrono>
#include <string> 

struct TrackedWindow
{
	HWND hWnd = nullptr;
	LPWSTR title = nullptr;
	std::chrono::system_clock::time_point startTracking;
	std::chrono::system_clock::duration trackTime = std::chrono::seconds(0);
	std::chrono::system_clock::duration lastNotifyTime = std::chrono::seconds(0);
	std::chrono::system_clock::time_point lastCheck;
	std::wstring target;
};

typedef TrackedWindow* LPTrackedWindow;
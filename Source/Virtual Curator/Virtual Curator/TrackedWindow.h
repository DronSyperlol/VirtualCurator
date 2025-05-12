#pragma once
#include <Windows.h>
#include <chrono>

struct TrackedWindow
{
	HWND hWnd;
	LPWSTR title;
	std::chrono::steady_clock::time_point startTracking;
};

typedef TrackedWindow* LPTrackedWindow;
#pragma once
#include <Windows.h>
#include <string>
#include "TrackedWindow.h"
#include "WindowBase.h"

void TrackForeground(LPWindowBase);


namespace _TrackProcesses {
	bool addToTrack(HWND);
	void freeMemoryTrackedWindow(LPTrackedWindow);

	void processTrigger(LPWindowBase, const LPTrackedWindow);
}
#pragma once
#include <Windows.h>
#include <string>

void TrackProcesses();


namespace TrackProcesses {
	void GetProcessName(HANDLE, std::wstring);
}
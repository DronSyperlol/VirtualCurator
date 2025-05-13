#include "TrackProcesses.h"
#include <chrono>
#include <thread>
#include <map>
#include <random>
#include "MainWindow.h"

constexpr std::chrono::system_clock::duration TRIGER_TIME = std::chrono::minutes(10);
constexpr std::chrono::system_clock::duration FORGOT_TIME = std::chrono::minutes(5);

static std::map<HWND, LPTrackedWindow> tracket;
static std::vector<std::wstring> targets
{
	L"- YouTube",
	L"| TikTok"
};
static bool functionCalled = false;

using namespace _TrackProcesses;

void TrackForeground(LPWindowBase notifyRecipient)
{
	if (functionCalled) throw "Is a singleton!";
	functionCalled = true;
	while (true) {
		HWND wnd = GetForegroundWindow();
		if (tracket.contains(wnd))
		{
			auto elem = tracket[wnd];
			elem->trackTime = std::chrono::system_clock::now() - elem->startTracking;
			elem->lastCheck = std::chrono::system_clock::now();
			if (elem->trackTime - elem->lastNotifyTime >= TRIGER_TIME) {
				elem->lastNotifyTime = elem->trackTime;
				processTrigger(notifyRecipient, elem);
			}
		}
		else {
			addToTrack(wnd);
		}
		auto threshold = std::chrono::system_clock::now() - FORGOT_TIME;
		auto iter = tracket.begin();
		while (iter != tracket.end()) {
			if (iter->second->lastCheck < threshold) {
				iter = tracket.erase(iter);
			}
			else {
				++iter;
			}
		}
		std::this_thread::sleep_for(std::chrono::seconds(10));
	}
}

bool _TrackProcesses::addToTrack(HWND wnd)
{
	const int buffer_size = 1024;
	auto elem = new TrackedWindow;
	ZeroMemory(elem, sizeof(TrackedWindow));
	elem->hWnd = wnd;
	elem->startTracking = std::chrono::system_clock::now();
	elem->lastCheck = std::chrono::system_clock::now();
	elem->title = new WCHAR[buffer_size];
	GetWindowText(wnd, elem->title, buffer_size);
	std::wstring title(elem->title);
	for (std::wstring target : targets) {
		if (title.find(target) != std::wstring::npos) {
			tracket[wnd] = elem;
			elem->target = target;
			return true;
		}
	}
	freeMemoryTrackedWindow(elem);
	return false;
}

void _TrackProcesses::freeMemoryTrackedWindow(LPTrackedWindow elem)
{
	delete[] elem->title;
	delete elem;
	elem = nullptr;
}

void _TrackProcesses::processTrigger(LPWindowBase wnd, const LPTrackedWindow elem)
{
	static std::vector<std::wstring> notificationsTemplates
	{
		L"Вы уже {time} сидите в {title}! Неужели вы отвлеклись?",
		L"Я подозреваю, что рабочий процесс нарушен. Вы провели {time} в {title}"
	};
	static std::wstring timeTrigger = L"{time}";
	static std::wstring titleTrigger = L"{title}";

	std::srand(std::chrono::system_clock::now().time_since_epoch().count());
	int notifyIndex = std::rand() % notificationsTemplates.size();
	std::wstring notifyMessage = notificationsTemplates.at(notifyIndex);

	std::wstring time = std::to_wstring(std::chrono::duration_cast<std::chrono::minutes>(elem->trackTime).count()) + L" минут";
	notifyMessage.replace(notifyMessage.find(timeTrigger), timeTrigger.size(), time);

	notifyMessage.replace(notifyMessage.find(titleTrigger), titleTrigger.size(), elem->target);
	LPCWSTR messagePointer = notifyMessage.c_str();
	wnd->sendMessage(WM_USER, (WPARAM)messagePointer, MASKOT_SAY);
}

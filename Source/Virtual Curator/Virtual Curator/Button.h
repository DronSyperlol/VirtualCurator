#pragma once

#include "WindowBase.h"

namespace VirtualCurator 
{
	class Button : public WindowBase {
	private:
		void (*_onClick)();
		LPCWSTR getClassName() const override;

		LRESULT onWindowCreated(HWND, WPARAM, LPARAM) const override;
		LRESULT onWindowDestroyed(HWND, WPARAM, LPARAM) const override;
		LRESULT onRawWndProc(HWND, UINT, WPARAM, LPARAM) const override;
	public:
		Button(HINSTANCE hInst, HWND parent, LPCWSTR buttonName, int x, int y, int width, int height, void(*onClick)());
	protected:
	};
}

#pragma once
#include <Windows.h> 
#include <map>

class WindowBase { //����������� �����
private:
public:
	static LRESULT routeEvents(HWND, UINT, WPARAM, LPARAM);

	WindowBase(HINSTANCE);
	virtual void show(bool) const;
	~WindowBase();
protected:
	HWND _hWnd;
	HINSTANCE _hInstance;
	virtual LPCWSTR getClassName() const = 0; //����� ��� ��������� ������ ����
	HWND initializeWindow(
		DWORD exStyle, LPCWSTR windowName, DWORD style,
		int x, int y, int width, int height, HWND parent, HMENU menu);

	// Events: 
	virtual LRESULT onRawWndProc(HWND, UINT, WPARAM, LPARAM) const;
	virtual LRESULT onWindowCreated(HWND, WPARAM, LPARAM) const = 0;
	virtual LRESULT onWindowDestroyed(HWND, WPARAM, LPARAM) const = 0;
};

typedef WindowBase* LPWindowBase;
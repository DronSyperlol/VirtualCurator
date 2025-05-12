#pragma once
#include <Windows.h> 
#include <map>

class WindowBase { //абстрактный класс
private:
public:
	static LRESULT routeEvents(HWND, UINT, WPARAM, LPARAM);

	WindowBase(HINSTANCE);
	virtual void show(bool) const;
	void move(int x, int y) const;
	void sendMessage(UINT, WPARAM, LPARAM) const;
	~WindowBase();
protected:
	HWND _hWnd;
	HINSTANCE _hInstance;
	virtual LPCWSTR getClassName() const = 0; //метод для получения класса окна
	HWND initializeWindow(
		DWORD exStyle, LPCWSTR windowName, DWORD style,
		int x, int y, int width, int height, HWND parent, HMENU menu);

	// Events: 
	virtual LRESULT onRawWndProc(HWND, UINT, WPARAM, LPARAM) const;
	virtual LRESULT onWindowCreate(HWND, WPARAM, LPARAM) const = 0;
	virtual LRESULT onWindowDestroy(HWND, WPARAM, LPARAM) const = 0;
	virtual LRESULT onCommand(HWND, WPARAM, LPARAM) const;
};

typedef WindowBase* LPWindowBase;
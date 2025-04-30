#pragma once
#include <Windows.h> 
#include <map>

class WindowBase { //абстрактный класс
private:
	static std::map<HWND, WindowBase*> _createdWindows;
	static WindowBase* getWindowByHWnd(HWND);
public:
	static LRESULT routeEvents(HWND, UINT, WPARAM, LPARAM);
	WindowBase(HINSTANCE);
	virtual void show(bool) const;
	~WindowBase();
protected:
	HWND _hWnd;
	HINSTANCE _hInstance;
	virtual LPCWSTR getClassName() const = 0; //метод для получения класса окна
	void initializeWindow(
		DWORD exStyle, LPCWSTR className, LPCWSTR windowName, DWORD style,
		int x, int y, int width, int height, HWND parent, HMENU menu, LPVOID lp);

	// Events: 
	virtual LRESULT onRawWndProc(UINT, WPARAM, LPARAM) const;
	virtual LRESULT onWindowCreated(WPARAM, LPARAM) const;
};

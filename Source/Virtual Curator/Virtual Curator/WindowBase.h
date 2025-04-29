#pragma once
#include <Windows.h> 
#include <map>

class WindowBase { //абстрактный класс
private:
	std::map<HWND, WindowBase*> _createdWindows;
public:
	virtual void show(bool) const;
	WindowBase(HINSTANCE);

	~WindowBase();
protected:
	HWND _hWnd;
	HINSTANCE _hInstance;
	virtual LPCWSTR getClassName() const = 0; //метод для получения класса окна
	void initializeWindow(DWORD exStyle, LPCWSTR className, LPCWSTR windowName, DWORD style,
		int x, int y, int width, int height, HWND parent, HMENU menu, LPVOID lp);
};

#pragma once
#include <Windows.h>

class Window
{
public:
	Window(int width, int height);
	~Window();


private:
	static LRESULT CALLBACK MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

	int mWidth;
	int mHeight;
	HINSTANCE mhInst;
	HWND mhWnd;
};

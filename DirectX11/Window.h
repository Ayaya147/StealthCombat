#pragma once
#include <Windows.h>
//#include "Keyboard.h"

class Window
{
public:
	Window(int width, int height);
	~Window();

	HWND GetHandle() const noexcept { return mhWnd; }
	int GetClientWidth() const noexcept { return mWidth; }
	int GetClientHeight() const noexcept { return mHeight; }

	static class Keyboard* keyboard;

private:
	static LRESULT CALLBACK MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

	int mWidth;
	int mHeight;
	HINSTANCE mhInst;
	HWND mhWnd;

};

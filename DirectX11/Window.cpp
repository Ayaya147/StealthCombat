#include "Window.h"

Window::Window(int width, int height)
	:
	mWidth(width),
	mHeight(height)
{
	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = MsgProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = mhInst;
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = "GameApp Window";

	if (!RegisterClassEx(&wc))
	{
		MessageBox(0, "RegisterClass Failed.", 0, 0);
	}

	RECT r = { 0, 0, mWidth, mHeight };
	AdjustWindowRect(&r, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

	mhWnd = CreateWindow(
		wc.lpszClassName, "GameApp",
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, mWidth, mHeight,
		nullptr, nullptr, mhInst, this
	);

	if (!mhWnd)
	{
		MessageBox(0, "CreateWindow Failed.", 0, 0);
	}

	ShowWindow(mhWnd, SW_SHOWDEFAULT);
	UpdateWindow(mhWnd);
}

Window::~Window()
{
}

LRESULT Window::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

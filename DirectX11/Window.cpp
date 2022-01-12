#include "Window.h"
#include "Keyboard.h"


Keyboard* Window::keyboard = nullptr;

Window::Window(int width, int height)
	:
	mWidth(width),
	mHeight(height),
	mhInst(GetModuleHandle(nullptr))
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

	RegisterClassEx(&wc);

#ifdef DEBUG
	RECT r = { 0, 0, mWidth, mHeight };
	AdjustWindowRect(&r, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);
	int widthApp = r.right - r.left;
	int heightApp = r.bottom - r.top;

	RECT rDesk = {};
	HWND hDesk = GetDesktopWindow();
	GetWindowRect(hDesk, &rDesk);
	int widthDesk = rDesk.right - rDesk.left;
	int heightDesk = rDesk.bottom - rDesk.top;

	mhWnd = CreateWindow(
		wc.lpszClassName, "GameApp",
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		(widthDesk - widthApp) / 2, (heightDesk - heightApp) / 2,
		widthApp, heightApp,
		nullptr, nullptr, mhInst, nullptr
	);
#else
	RECT rDesk = {};
	HWND hDesk = GetDesktopWindow();
	GetWindowRect(hDesk, &rDesk);
	mWidth = rDesk.right - rDesk.left;
	mHeight = rDesk.bottom - rDesk.top;

	mhWnd = CreateWindow(
		wc.lpszClassName, "GameApp",
		WS_POPUP, 0, 0, mWidth, mHeight,
		nullptr, nullptr, mhInst, nullptr
	);
#endif

	ShowWindow(mhWnd, SW_SHOWDEFAULT);
	UpdateWindow(mhWnd);

	//RAWINPUTDEVICE rid;
	//rid.usUsagePage = 0x01; // mouse page
	//rid.usUsage = 0x02; // mouse usage
	//rid.dwFlags = 0;
	//rid.hwndTarget = nullptr;
	//if (RegisterRawInputDevices(&rid, 1, sizeof(rid)) == FALSE)
	//{
	//}

	keyboard = new Keyboard();
}

Window::~Window()
{
	DestroyWindow(mhWnd);
}

LRESULT Window::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;

	case WM_KILLFOCUS:
		keyboard->ClearState();

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			return 0;
		}

	case WM_SYSKEYDOWN:
		if (!(lParam & 0x40000000) || keyboard->AutorepeatIsEnabled())
		{
			keyboard->OnKeyPressed(static_cast<unsigned char>(wParam));
		}
		break;

	case WM_KEYUP:
	case WM_SYSKEYUP:
		keyboard->OnKeyReleased(static_cast<unsigned char>(wParam));
		break;

		
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

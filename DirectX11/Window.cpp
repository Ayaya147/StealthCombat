#include "Window.h"
#include "InputSystem.h"
#include "Keyboard.h"
#include "imgui/imgui_impl_win32.h"

InputSystem* Window::mInput = nullptr;

Window::Window(int width, int height, InputSystem* input)
	:
	mWidth(width),
	mHeight(height),
	mhInst(GetModuleHandle(nullptr))
{
#ifdef NDEBUG
	ShowCursor(FALSE);
#endif

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
	int deskHeight = rDesk.bottom - rDesk.top;
	int deskWidth = rDesk.right - rDesk.left;

	if (deskWidth / deskHeight < 16.0f / 9.0f)
	{
		int w = deskWidth;
		int h = static_cast<int>(w / 16.0f * 9.0f);
		mhWnd = CreateWindow(
			wc.lpszClassName, "GameApp",
			WS_POPUP, 0, (deskHeight - h) / 2, w, h,
			nullptr, nullptr, mhInst, nullptr
		);
	}
	else
	{
		int h = deskHeight;
		int w = static_cast<int>(h / 9.0f * 16.0f);
		mhWnd = CreateWindow(
			wc.lpszClassName, "GameApp",
			WS_POPUP, (deskWidth - w) / 2, 0, w, h,
			nullptr, nullptr, mhInst, nullptr
		);
	}
#endif

	ShowWindow(mhWnd, SW_SHOWDEFAULT);
	UpdateWindow(mhWnd);

	ImGui_ImplWin32_Init(mhWnd);
	mInput = input;
}

Window::~Window()
{
	ImGui_ImplWin32_Shutdown();
	DestroyWindow(mhWnd);
}

DirectX::XMFLOAT3 Window::GetMouseCursorPos()
{
	POINT p = {};
	GetCursorPos(&p);
	ScreenToClient(mhWnd, &p);

	float x = static_cast<float>(p.x) / mWidth * 1920 - 1920 /2.0f;
	float y = static_cast<float>(p.y) / mHeight * 1080 - 1080 /2.0f;

	return DirectX::XMFLOAT3{ x,y,0 };
}

LRESULT Window::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	Keyboard* keyboard = mInput->GetKeyboard();

	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
	{
		return true;
	}
	const auto imio = ImGui::GetIO();

	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;

	case WM_KILLFOCUS:
		keyboard->ClearState();
		break;

	case WM_KEYDOWN:
		//switch (wParam)
		//{
		//case VK_ESCAPE:
		//	PostQuitMessage(0);
		//	return 0;
		//}
	case WM_SYSKEYDOWN:
		if (imio.WantCaptureKeyboard)
		{
			//break;
		}
		keyboard->KeyDown(static_cast<unsigned char>(wParam));
		break;

	case WM_KEYUP:
	case WM_SYSKEYUP:
		if (imio.WantCaptureKeyboard)
		{
			//break;
		}
		keyboard->KeyUp(static_cast<unsigned char>(wParam));
		break;

	case WM_CHAR:
		if (imio.WantCaptureKeyboard)
		{
			//break;
		}
		break;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

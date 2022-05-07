#pragma once
#include <Windows.h>
#include <DirectXMath.h>

class Window
{
public:
	Window(int width, int height, class InputSystem* input);
	~Window();

	HWND GetHandle() const noexcept { return mhWnd; }
	int GetClientWidth() const noexcept { return mWidth; }
	int GetClientHeight() const noexcept { return mHeight; }
	DirectX::XMFLOAT3 GetMouseCursorPos();

private:
	static LRESULT CALLBACK MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static class InputSystem* mInput;

	int mWidth;
	int mHeight;
	HINSTANCE mhInst;
	HWND mhWnd;
};

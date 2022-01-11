#pragma once
#include <dinput.h>

class InputSystem
{
public:
	InputSystem();
	~InputSystem();

private:
	LPDIRECTINPUT8 mDInput;
};

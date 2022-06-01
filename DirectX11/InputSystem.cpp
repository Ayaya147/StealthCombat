#include "InputSystem.h"
#include "GamePad.h"
#include "Keyboard.h"

GamePad* InputSystem::mPad = nullptr;
Keyboard* InputSystem::mKeyboard = nullptr;

InputSystem::InputSystem()
	:
	mIsGamePad(false)
{
	mPad = new GamePad();
	mKeyboard = new Keyboard();
}

InputSystem::~InputSystem()
{
	delete mPad;
	delete mKeyboard;
}

void InputSystem::Update()
{
	mPad->Update();
	mKeyboard->Update();

	if (mPad->GetIsGamePad())
	{
		mIsGamePad = true;
	}
	else
	{
		mIsGamePad = false;
	}
}

bool InputSystem::GetPlayerLeftTurn()
{
	if (mIsGamePad)
	{
		return mPad->GetThumbLeftX() < 0;
	}
	else
	{
		return mKeyboard->GetKeyValue('A');
	}
}

bool InputSystem::GetPlayerRightTurn()
{
	if (mIsGamePad)
	{
		return mPad->GetThumbLeftX() > 0;
	}
	else
	{
		return mKeyboard->GetKeyValue('D');
	}
}

bool InputSystem::GetPlayerAccel()
{
	if (mIsGamePad)
	{
		return mPad->GetRightTrigger();
	}
	else
	{
		return mKeyboard->GetKeyValue('W');
	}
}

bool InputSystem::GetPlayerDecel()
{
	if (mIsGamePad)
	{
		return mPad->GetLeftTrigger();
	}
	else
	{
		return mKeyboard->GetKeyValue('S');
	}
}

bool InputSystem::GetPlayerEmitMissile()
{
	if (mIsGamePad)
	{
		return mPad->GetButtonState(XINPUT_GAMEPAD_Y) == ButtonState::EPressed;
	}
	else
	{
		return mKeyboard->GetKeyState(VK_SPACE) == ButtonState::EPressed;
	}
}

bool InputSystem::GetCameraInput()
{
	if (mIsGamePad)
	{
		return mPad->GetThumbRightY() < 0;
	}
	else
	{
		return mKeyboard->GetKeyValue('R');
	}
}

bool InputSystem::GetScenePause()
{
	if (mIsGamePad)
	{
		return mPad->GetButtonState(XINPUT_GAMEPAD_START) == ButtonState::EPressed;
	}
	else
	{
		return mKeyboard->GetKeyState(VK_ESCAPE) == ButtonState::EPressed;
	}
}

bool InputSystem::GetX()
{
	if (mIsGamePad)
	{
		return mPad->GetButtonState(XINPUT_GAMEPAD_X) == ButtonState::EPressed;
	}
	else
	{
		return mKeyboard->GetKeyState('X') == ButtonState::EPressed;
	}
}

bool InputSystem::GetY()
{
	if (mIsGamePad)
	{
		return mPad->GetButtonState(XINPUT_GAMEPAD_Y) == ButtonState::EPressed;
	}
	else
	{
		return mKeyboard->GetKeyState('Y') == ButtonState::EPressed;
	}
}

bool InputSystem::GetA()
{
	if (mIsGamePad)
	{
		return mPad->GetButtonState(XINPUT_GAMEPAD_A) == ButtonState::EPressed;
	}
	else
	{
		return mKeyboard->GetKeyState('A') == ButtonState::EPressed;
	}
}

bool InputSystem::GetB()
{
	if (mIsGamePad)
	{
		return mPad->GetButtonState(XINPUT_GAMEPAD_B) == ButtonState::EPressed;
	}
	else
	{
		return mKeyboard->GetKeyState('B') == ButtonState::EPressed;
	}
}

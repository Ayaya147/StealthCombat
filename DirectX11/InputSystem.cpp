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

bool InputSystem::GetCameraChange()
{
	if (mIsGamePad)
	{
		return mPad->GetButtonState(XINPUT_GAMEPAD_A) == ButtonState::EPressed;
	}
	else
	{
		return mKeyboard->GetKeyState('1') == ButtonState::EPressed;
	}
}

bool InputSystem::GetScenePause()
{
	if (mIsGamePad)
	{
		return mPad->GetButtonState(XINPUT_GAMEPAD_RIGHT_THUMB) == ButtonState::EPressed;
	}
	else
	{
		return mKeyboard->GetKeyState(VK_ESCAPE) == ButtonState::EPressed;
	}
}

bool InputSystem::GetSceneBack()
{
	if (mIsGamePad)
	{
		return mPad->GetButtonState(XINPUT_GAMEPAD_RIGHT_THUMB) == ButtonState::EPressed;
	}
	else
	{
		return mKeyboard->GetKeyState(VK_RETURN) == ButtonState::EPressed;
	}
}

bool InputSystem::GetSceneChangeEnter()
{
	if (mIsGamePad)
	{
		return mPad->GetButtonState(XINPUT_GAMEPAD_RIGHT_THUMB) == ButtonState::EPressed;
	}
	else
	{
		return mKeyboard->GetKeyState(VK_RETURN) == ButtonState::EPressed;
	}
}

bool InputSystem::GetSceneChangeSpace()
{
	if (mIsGamePad)
	{
		return mPad->GetButtonState(XINPUT_GAMEPAD_LEFT_THUMB) == ButtonState::EPressed;
	}
	else
	{
		return mKeyboard->GetKeyState(VK_SPACE) == ButtonState::EPressed;
	}
}

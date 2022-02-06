#include "GamePad.h"
#include "InputSystem.h"
#include <cmath>

GamePad::GamePad()
	:
	mIsGamePad(false)
{
	ZeroMemory(&mLastState, sizeof(XINPUT_STATE));
	ZeroMemory(&mState, sizeof(XINPUT_STATE));
	ZeroMemory(&mVibration, sizeof(XINPUT_VIBRATION));
}

GamePad::~GamePad()
{
	ZeroMemory(&mLastState, sizeof(XINPUT_STATE));
	ZeroMemory(&mState, sizeof(XINPUT_STATE));
	ZeroMemory(&mVibration, sizeof(XINPUT_VIBRATION));
	XInputSetState(0, &mVibration);
}

void GamePad::Update()
{
	mLastState = mState;
	XInputSetState(0, &mVibration);

	DWORD dwResult = XInputGetState(0, &mState);
	if (dwResult == ERROR_SUCCESS)
	{
		mIsGamePad = true;
	}
	else
	{
		mIsGamePad = false;
		return;
	}

	if (abs(mState.Gamepad.sThumbLX) < mThumbDeadZone)
	{
		mState.Gamepad.sThumbLX = 0;
	}
	if (abs(mState.Gamepad.sThumbLY) < mThumbDeadZone)
	{
		mState.Gamepad.sThumbLY = 0;
	}
	if (abs(mState.Gamepad.sThumbRX) < mThumbDeadZone)
	{
		mState.Gamepad.sThumbRX = 0;
	}
	if (abs(mState.Gamepad.sThumbRY) < mThumbDeadZone)
	{
		mState.Gamepad.sThumbRY = 0;
	}
	if (abs(mState.Gamepad.bLeftTrigger) < mTriggerDeadZone)
	{
		mState.Gamepad.bLeftTrigger = 0;
	}
	if (abs(mState.Gamepad.bRightTrigger) < mTriggerDeadZone)
	{
		mState.Gamepad.bRightTrigger = 0;
	}
}

int GamePad::GetThumbLeftX()
{
	return mState.Gamepad.sThumbLX;
}

int GamePad::GetThumbLeftY()
{
	return mState.Gamepad.sThumbLY;
}

int GamePad::GetThumbRightX()
{
	return mState.Gamepad.sThumbRX;
}

int GamePad::GetThumbRightY()
{
	return mState.Gamepad.sThumbRY;
}

int GamePad::GetLeftTrigger()
{
	return mState.Gamepad.bLeftTrigger;
}

int GamePad::GetRightTrigger()
{
	return mState.Gamepad.bRightTrigger;
}

bool GamePad::GetButtonValue(DWORD button) const
{
	return (button & mState.Gamepad.wButtons);
}

ButtonState GamePad::GetButtonState(DWORD button) const
{
	if (button & mLastState.Gamepad.wButtons)
	{
		if (button & mState.Gamepad.wButtons)
		{
			return ButtonState::EHeld;
		}
		else
		{
			return ButtonState::EReleased;
		}
	}
	else
	{
		if (button & mState.Gamepad.wButtons)
		{
			return ButtonState::EPressed;
		}
		else
		{
			return ButtonState::ENone;
		}
	}
}

void GamePad::SetLeftVibration(int speed)
{
	speed %= 256;
	mVibration.wLeftMotorSpeed = ((speed + 1) * 256) - 1;
}

void GamePad::SetRightVibration(int speed)
{
	speed %= 256;
	mVibration.wRightMotorSpeed = ((speed + 1) * 256) - 1;
}

void GamePad::SetVibration(int speed)
{
	SetLeftVibration(speed);
	SetRightVibration(speed);
}

void GamePad::StopVibration()
{
	mVibration.wLeftMotorSpeed = 0;
	mVibration.wRightMotorSpeed = 0;
}

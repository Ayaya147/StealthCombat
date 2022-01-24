#include "GamePad.h"
#include <cmath>

#define DEADZONE (15000)

GamePad::GamePad()
{
	ZeroMemory(&mController, sizeof(ControllerState));
}

GamePad::~GamePad()
{
	ZeroMemory(&mController, sizeof(ControllerState));
	XInputSetState(0, &mController.mVibration);
}

void GamePad::Update()
{
	XInputSetState(0, &mController.mVibration);
	mController.mLastState = mController.mState;
	XInputGetState(0, &mController.mState);
	mController.mTrigger.Gamepad.wButtons = ((mController.mLastState.Gamepad.wButtons ^ mController.mState.Gamepad.wButtons) & mController.mState.Gamepad.wButtons);

	if (abs(mController.mState.Gamepad.sThumbLX) < DEADZONE)
	{
		mController.mState.Gamepad.sThumbLX = 0;
	}
	if (abs(mController.mState.Gamepad.sThumbLY) < DEADZONE)
	{
		mController.mState.Gamepad.sThumbLY = 0;
	}
	if (abs(mController.mState.Gamepad.sThumbRX) < DEADZONE)
	{
		mController.mState.Gamepad.sThumbRX = 0;
	}
	if (abs(mController.mState.Gamepad.sThumbRY) < DEADZONE)
	{
		mController.mState.Gamepad.sThumbRY = 0;
	}
	if (abs(mController.mState.Gamepad.bLeftTrigger) < 30)
	{
		mController.mState.Gamepad.bLeftTrigger = 0;
	}
	if (abs(mController.mState.Gamepad.bRightTrigger) < 30)
	{
		mController.mState.Gamepad.bRightTrigger = 0;
	}
}

int GamePad::GetThumbLeftX()
{
	return mController.mState.Gamepad.sThumbLX;
}

int GamePad::GetThumbLeftY()
{
	return mController.mState.Gamepad.sThumbLY;
}

int GamePad::GetThumbRightX()
{
	return mController.mState.Gamepad.sThumbRX;
}

int GamePad::GetThumbRightY()
{
	return mController.mState.Gamepad.sThumbRY;
}

int GamePad::GetLeftTrigger()
{
	return mController.mState.Gamepad.bLeftTrigger;
}

int GamePad::GetRightTrigger()
{
	return mController.mState.Gamepad.bRightTrigger;
}

void GamePad::SetLeftVibration(int speed)
{
	speed %= 256;
	mController.mVibration.wLeftMotorSpeed = ((speed + 1) * 256) - 1;
}

void GamePad::SetRightVibration(int speed)
{
	speed %= 256;
	mController.mVibration.wRightMotorSpeed = ((speed + 1) * 256) - 1;
}

void GamePad::SetVibration(int speed)
{
	SetLeftVibration(speed);
	SetRightVibration(speed);
}

void GamePad::StopVibration()
{
	mController.mVibration.wLeftMotorSpeed = 0;
	mController.mVibration.wRightMotorSpeed = 0;
}

bool GamePad::IsButtonPressed(DWORD button)
{
	return (button & mController.mState.Gamepad.wButtons);
}

bool GamePad::IsButtonTriggered(DWORD button)
{
	return (button & mController.mTrigger.Gamepad.wButtons);
}

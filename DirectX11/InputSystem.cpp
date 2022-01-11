#include "InputSystem.h"
#include <cmath>

#define DEADZONE (8200)

InputSystem::InputSystem()
{
	ZeroMemory(&mController, sizeof(ControllerState));
}

InputSystem::~InputSystem()
{
	ZeroMemory(&mController, sizeof(ControllerState));
	XInputSetState(0, &mController.mVibration);
}

void InputSystem::Update()
{
	XInputSetState(0, &mController.mVibration);

	mController.mLastState = mController.mState;

	XInputGetState(0, &mController.mState);

	mController.mTrigger.Gamepad.wButtons = ((mController.mLastState.Gamepad.wButtons ^ mController.mState.Gamepad.wButtons) & mController.mState.Gamepad.wButtons);

	if (fabs(mController.mState.Gamepad.sThumbLX) < DEADZONE)
	{
		mController.mState.Gamepad.sThumbLX = 0;
	}
	if (fabs(mController.mState.Gamepad.sThumbLY) < DEADZONE)
	{
		mController.mState.Gamepad.sThumbLY = 0;
	}
	if (fabs(mController.mState.Gamepad.sThumbRX) < DEADZONE)
	{
		mController.mState.Gamepad.sThumbRX = 0;
	}
	if (fabs(mController.mState.Gamepad.sThumbRY) < DEADZONE)
	{
		mController.mState.Gamepad.sThumbRY = 0;
	}
}

int InputSystem::GetThumbLeftX()
{
	return mController.mState.Gamepad.sThumbLX;
}

int InputSystem::GetThumbLeftY()
{
	return mController.mState.Gamepad.sThumbLY;
}

int InputSystem::GetThumbRightX()
{
	return mController.mState.Gamepad.sThumbRX;
}

int InputSystem::GetThumbRightY()
{
	return mController.mState.Gamepad.sThumbRY;
}

int InputSystem::GetLeftTrigger()
{
	return mController.mState.Gamepad.bLeftTrigger;
}

int InputSystem::GetRightTrigger()
{
	return mController.mState.Gamepad.bRightTrigger;
}

void InputSystem::SetLeftVibration(int speed)
{
	speed %= 256;
	mController.mVibration.wLeftMotorSpeed = ((speed + 1) * 256) - 1;
}

void InputSystem::SetRightVibration(int speed)
{
	speed %= 256;
	mController.mVibration.wRightMotorSpeed = ((speed + 1) * 256) - 1;
}

void InputSystem::SetVibration(int speed)
{
	SetLeftVibration(speed);
	SetRightVibration(speed);
}

void InputSystem::StopVibration()
{
	mController.mVibration.wLeftMotorSpeed = 0;
	mController.mVibration.wRightMotorSpeed = 0;
}

bool InputSystem::IsButtonPressed(DWORD button)
{
	return (button & mController.mState.Gamepad.wButtons);
}

bool InputSystem::IsButtonTriggered(DWORD button)
{
	return (button & mController.mTrigger.Gamepad.wButtons);
}

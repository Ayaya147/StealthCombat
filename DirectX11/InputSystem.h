#pragma once
#include <Windows.h>
#include <Xinput.h>

class InputSystem
{
public:
	InputSystem();
	~InputSystem();

	void Update();

	int GetThumbLeftX();
	int GetThumbLeftY();
	int GetThumbRightX();
	int GetThumbRightY();
	int GetLeftTrigger();
	int GetRightTrigger();

	void SetLeftVibration(int speed);
	void SetRightVibration(int speed);
	void SetVibration(int speed);
	void StopVibration();

	bool IsButtonPressed(DWORD button);
	bool IsButtonTriggered(DWORD button);

private:
	struct ControllerState
	{
		XINPUT_STATE		mLastState;
		XINPUT_STATE		mState;
		XINPUT_STATE		mTrigger;
		XINPUT_VIBRATION	mVibration;
	};

	ControllerState mController;
};

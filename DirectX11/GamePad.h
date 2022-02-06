#pragma once
#include <Windows.h>
#include <Xinput.h>

class GamePad
{
public:
	GamePad();
	~GamePad();

	void Update();

	int GetThumbLeftX();
	int GetThumbLeftY();
	int GetThumbRightX();
	int GetThumbRightY();
	int GetLeftTrigger();
	int GetRightTrigger();

	bool GetButtonValue(DWORD button) const;
	enum class ButtonState GetButtonState(DWORD button) const;
	bool GetIsGamePad() const { return mIsGamePad; }

	void SetLeftVibration(int speed);
	void SetRightVibration(int speed);
	void SetVibration(int speed);
	void StopVibration();

private:
	static constexpr int mThumbDeadZone = 15000;
	static constexpr int mTriggerDeadZone = 30;

	XINPUT_STATE		mLastState;
	XINPUT_STATE		mState;
	XINPUT_VIBRATION	mVibration;
	bool mIsGamePad;
};

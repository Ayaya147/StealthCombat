#pragma once

enum class ButtonState
{
	ENone,
	EPressed,
	EReleased,
	EHeld
};

class InputSystem
{
public:
	InputSystem();
	~InputSystem();

	void Update();

	bool GetPlayerLeftTurn();
	bool GetPlayerRightTurn();
	bool GetPlayerAccel();
	bool GetPlayerDecel();
	bool GetPlayerEmitMissile();
	bool GetCameraInput();

	bool GetScenePause();
	bool GetX();
	bool GetY();
	bool GetA();
	bool GetB();

	class Keyboard* GetKeyboard() const { return mKeyboard; }
	class GamePad* GetPad() const { return mPad; }

private:
	static class GamePad* mPad;
	static class Keyboard* mKeyboard;
	bool mIsGamePad;
};

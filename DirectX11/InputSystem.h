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

	class Keyboard* GetKeyboard() const { return mKeyboard; }
	class GamePad* GetPad() const { return mPad; }

private:
	static class GamePad* mPad;
	static class Keyboard* mKeyboard;
};

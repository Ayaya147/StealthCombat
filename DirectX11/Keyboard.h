#pragma once
#include <vector>

class Keyboard
{
	friend class Window;

public:
	Keyboard();
	~Keyboard();

	void Update();

	bool GetKeyValue(unsigned char keycode) const;
	enum class ButtonState GetKeyState(unsigned char keycode) const;

private:
	void KeyDown(unsigned char keycode);
	void KeyUp(unsigned char keycode);
	void ClearState();

	static constexpr unsigned int mKeys = 256;
	std::vector<int> mKeyStates;
	std::vector<int> mCounts;
};

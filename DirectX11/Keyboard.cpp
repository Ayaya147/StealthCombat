#include "Keyboard.h"
#include "InputSystem.h"

Keyboard::Keyboard()
	:
	mKeyStates(std::vector<int>(mKeys, 0)),
	mCounts(std::vector<int>(mKeys, 0))
{
}

Keyboard::~Keyboard()
{
}

void Keyboard::Update()
{
	for (int i = 0; i < mKeys; i++)
	{
		if (mKeyStates[i] && mCounts[i] <= 0)
		{
			mCounts[i] = 1;
		}
		else if (mKeyStates[i])
		{
			mCounts[i]++;
		}
		else if (!mKeyStates[i] && mCounts[i] > 0)
		{
			mCounts[i] = 0;
		}
		else
		{
			mCounts[i]--;
		}
	}
}

bool Keyboard::GetKeyValue(unsigned char keycode) const
{
	return mCounts[keycode] >= 1;
}

ButtonState Keyboard::GetKeyState(unsigned char keycode) const
{
	if (mCounts[keycode] > 1)
	{
		return ButtonState::EHeld;
	}
	else if (mCounts[keycode] == 1)
	{
		return ButtonState::EPressed;
	}
	else if (mCounts[keycode] == 0)
	{
		return ButtonState::EReleased;
	}
	else
	{
		return ButtonState::ENone;
	}
}

void Keyboard::KeyDown(unsigned char keycode)
{
	mKeyStates[keycode] = 1;
}

void Keyboard::KeyUp(unsigned char keycode)
{
	mKeyStates[keycode] = 0;
}

void Keyboard::ClearState()
{
	mKeyStates = std::vector<int>(mKeys, 0);
}

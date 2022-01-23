#include "InputSystem.h"
#include "GamePad.h"
#include "Keyboard.h"

GamePad* InputSystem::mPad = nullptr;
Keyboard* InputSystem::mKeyboard = nullptr;

InputSystem::InputSystem()
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
}

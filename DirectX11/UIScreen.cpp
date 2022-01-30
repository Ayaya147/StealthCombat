#include "UIScreen.h"
#include "Renderer.h"
#include "Bindable.h"

std::vector<Bindable*> UIScreen::mBinds;
bool UIScreen::mIsStaticInit = false;

UIScreen::UIScreen()
{
	if (!mIsStaticInit)
	{
		mIsStaticInit = true;
	}
}

UIScreen::~UIScreen()
{
}

void UIScreen::Draw(Renderer* renderer)
{
}

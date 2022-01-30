#pragma once
#include <vector>

class UIScreen
{
public:
	UIScreen();
	virtual ~UIScreen();

	virtual void Draw(class Renderer* renderer);

private:
	static std::vector<class Bindable*> mBinds;
	static bool mIsStaticInit;
};

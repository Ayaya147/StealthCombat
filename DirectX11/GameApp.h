#pragma once
#include <memory>

class GameApp
{
public:
	GameApp();
	~GameApp();

	int Run();

private:
	std::unique_ptr<class Window> mWindow;
};

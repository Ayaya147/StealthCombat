#pragma once
#include <memory>

class GameApp
{
public:
	GameApp();
	~GameApp();

	int Run();

	class Renderer* GetRenderer() const { return mRenderer.get(); }

private:
	std::unique_ptr<class Window> mWindow;
	std::unique_ptr<class Renderer> mRenderer;
	std::unique_ptr<class SceneManager> mSceneManager;
};

#pragma once

class GameApp
{
public:
	GameApp();
	~GameApp();

	int Run();

	class Renderer* GetRenderer() const { return mRenderer; }

private:
	class Window* mWindow;
	class Renderer* mRenderer;
	class SceneManager* mSceneManager;
};

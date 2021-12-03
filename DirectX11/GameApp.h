#pragma once

class Window;
class Renderer;
class SceneManager;

class GameApp
{
public:
	GameApp();
	~GameApp();

	int Run();

	Renderer* GetRenderer() const { return mRenderer; }

private:
	Window* mWindow;
	Renderer* mRenderer;
	SceneManager* mSceneManager;
};

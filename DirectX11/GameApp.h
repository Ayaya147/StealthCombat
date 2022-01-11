#pragma once

class InputSystem;
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
	InputSystem* mInputSystem;
	Renderer* mRenderer;
	SceneManager* mSceneManager;
};

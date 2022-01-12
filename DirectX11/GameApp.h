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

	Window* GetWindow() const { return mWindow; }
	Renderer* GetRenderer() const { return mRenderer; }
	InputSystem* GetInputSystem() const { return mInputSystem; }

private:
	Window* mWindow;
	InputSystem* mInputSystem;
	Renderer* mRenderer;
	SceneManager* mSceneManager;
};

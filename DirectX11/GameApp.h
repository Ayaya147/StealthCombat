#pragma once

class GameApp
{
public:
	GameApp();
	~GameApp();

	int Run();

	class Window* GetWindow() const { return mWindow; }
	class Renderer* GetRenderer() const { return mRenderer; }
	class InputSystem* GetInputSystem() const { return mInputSystem; }

private:
	class Window* mWindow;
	class InputSystem* mInputSystem;
	class Renderer* mRenderer;
	class SceneManager* mSceneManager;
};

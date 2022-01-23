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
	static class InputSystem* mInputSystem;
	static class Window* mWindow;
	static class Renderer* mRenderer;
	static class SceneManager* mSceneManager;
};

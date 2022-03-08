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
	class AudioSystem* GetAudioSystem() const { return mAudioSystem; }

private:
	static class ImguiManager* mImgui;
	static class InputSystem* mInputSystem;
	static class AudioSystem* mAudioSystem;
	static class Window* mWindow;
	static class Renderer* mRenderer;
	static class SceneManager* mSceneManager;
};

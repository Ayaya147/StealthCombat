#pragma once
#include <vector>

class BaseScene
{
public:
	enum class SceneState
	{
		EPlay,
		EPaused,
		EQuit
	};

	BaseScene(class SceneManager* sm, const class Parameter& parameter);
	virtual ~BaseScene();

	virtual void ProcessInput();
	virtual void Update();
	virtual void GenerateOutput();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);
	void PushUI(class UIScreen* screen);

	void SetSceneState(SceneState state) { mSceneState = state; }
	class Renderer* GetRenderer();
	class InputSystem* GetInputSystem();
	class Window* GetWindow();
	class SceneManager* GetSceneManager() const { return mSceneManager; }
	class Fade* GetFade() const { return mFade; }
	float GetGameTime() const { return mGameTime; }
	float GetDeltaTime() const { return mDeltaTime; }
	SceneState GetSceneState() const { return mSceneState; }
	const std::vector<class UIScreen*>& GetUIStack() const { return mUIStack; }

private:
	std::vector<class Actor*> mActors;
	std::vector<class Actor*> mPendingActors;
	std::vector<class UIScreen*> mUIStack;

	class Timer* mTimer;
	class Fade* mFade;
	class SceneManager* mSceneManager;
	SceneState mSceneState;
	bool mUpdatingActors;
	float mGameTime;
	float mDeltaTime;
};

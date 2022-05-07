#pragma once
#include <vector>

class BaseScene
{
public:
	enum class SceneState
	{
		EPlay,
		EPaused,
		EGameEnd,
		EQuit
	};

	BaseScene(class SceneManager* sm, const class Parameter& parameter);
	virtual ~BaseScene();

	virtual void ProcessInput();
	virtual void Update();
	virtual void GenerateOutput();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);
	void PushUI(class PauseScreen* screen);

	void SetSceneState(SceneState state) { mSceneState = state; }
	void SetParticleManager(class ParticleManager* pm) { mParticleManager = pm; }
	class Renderer* GetRenderer();
	class InputSystem* GetInputSystem();
	class AudioSystem* GetAudioSystem();
	class Window* GetWindow();
	class SceneManager* GetSceneManager() const { return mSceneManager; }
	class ParticleManager* GetParticleManager() const { return mParticleManager; }
	class Fade* GetFade() const { return mFade; }
	float GetGameTime() const { return mGameTime; }
	float GetDeltaTime() const { return mDeltaTime; }
	SceneState GetSceneState() const { return mSceneState; }
	const std::vector<class PauseScreen*>& GetPauseUIStack() const { return mPauseUIStack; }

private:
	std::vector<class Actor*> mActors;
	std::vector<class Actor*> mPendingActors;
	std::vector<class PauseScreen*> mPauseUIStack;

	class Timer* mTimer;
	class Fade* mFade;
	class SceneManager* mSceneManager;
	class ParticleManager* mParticleManager;
	SceneState mSceneState;
	bool mUpdatingActors;
	float mGameTime;
	float mDeltaTime;
};

#pragma once
#include <vector>

class BaseScene
{
public:
	BaseScene(class SceneManager* sm, const class Parameter& parameter);
	virtual ~BaseScene();

	virtual void ProcessInput();
	virtual void Update();
	virtual void GenerateOutput();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	class Renderer* GetRenderer();
	class InputSystem* GetInputSystem();
	class Window* GetWindow();
	class SceneManager* GetSceneManager() const { return mSceneManager; }
	float GetGameTime() const { return mGameTime; }
	float GetDeltaTime() const { return mDeltaTime; }

private:
	std::vector<class Actor*> mActors;
	std::vector<class Actor*> mPendingActors;
	class Timer* mTimer;
	class SceneManager* mSceneManager;
	bool mUpdatingActors;
	float mGameTime;
	float mDeltaTime;
};

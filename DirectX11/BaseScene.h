#pragma once
#include <vector>

class SceneManager;
class Parameter;
class Actor;
class PointLightActor;
class Timer;

class BaseScene
{
public:
	BaseScene(SceneManager* sm, const Parameter& parameter);
	virtual ~BaseScene();

	virtual void ProcessInput();
	virtual void Update();
	virtual void GenerateOutput();

	void AddActor(Actor* actor);
	void RemoveActor(Actor* actor);

	SceneManager* GetSceneManager() const { return mSceneManager; }
	virtual PointLightActor* GetLight() const { return nullptr; };

protected:
	std::vector<class Actor*> mActors;
	std::vector<class Actor*> mPendingActors;
	Timer* mTimer;
	SceneManager* mSceneManager;
	bool mUpdatingActors;
};

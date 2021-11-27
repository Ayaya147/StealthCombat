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

	class SceneManager* GetSceneManager() const { return mSceneManager; }
	virtual class PointLightActor* GetLight() const = 0;

protected:
	std::vector<class Actor*> mActors;
	std::vector<class Actor*> mPendingActors;
	class Timer* mTimer;
	bool mUpdatingActors;

	class SceneManager* mSceneManager;
};

#pragma once
#include <vector>
class AbstractScene
{
public:
	AbstractScene(class SceneManager* sm, const class Parameter& parameter);
	virtual ~AbstractScene();

	virtual void ProcessInput();
	virtual void Update();
	virtual void GenerateOutput();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	class SceneManager* GetSceneManager() const { return mSceneManager; }

protected:
	std::vector<class Actor*> mActors;
	std::vector<class Actor*> mPendingActors;

	class SceneManager* mSceneManager;
	class Timer* mTimer;

	bool mUpdatingActors;
};

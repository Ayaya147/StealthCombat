#pragma once

class AbstractScene
{
public:
	AbstractScene(class SceneManager* sm, const class Parameter& parameter);
	virtual ~AbstractScene();

	virtual void ProcessInput() = 0;
	virtual void Update() = 0;
	virtual void GenerateOutput() = 0;

	class SceneManager* GetSceneManager() const { return mSceneManager; }

protected:
	class SceneManager* mSceneManager;
};

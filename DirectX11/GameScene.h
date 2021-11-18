#pragma once
#include <vector>
#include "AbstractScene.h"

class GameScene : public AbstractScene
{
public:
	GameScene(class SceneManager* sm, const class Parameter& parameter);
	~GameScene();

	void ProcessInput() override;
	void Update() override;
	void GenerateOutput() override;

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

private:
	std::vector<class Actor*> mActors;
};

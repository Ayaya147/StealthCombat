#pragma once
#include <vector>
#include <memory>
#include "AbstractScene.h"

class GameScene : public AbstractScene
{
public:
	using AbstractScene::AbstractScene;
	~GameScene();

	void ProcessInput() override;
	void Update() override;
	void GenerateOutput() override;

	void AddActor(class Actor* actor);

private:
	std::vector<std::shared_ptr<class Actor>> mActors;
};

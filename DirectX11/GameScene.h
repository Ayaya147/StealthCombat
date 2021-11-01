#pragma once
#include "AbstractScene.h"

class GameScene : public AbstractScene
{
public:
	using AbstractScene::AbstractScene;
	~GameScene();

	void ProcessInput() override;
	void Update() override;
	void GenerateOutput() override;

private:

};

#pragma once
#include <vector>
#include "BaseScene.h"

class GameScene : public BaseScene
{
public:
	GameScene(SceneManager* sm, const Parameter& parameter);
	~GameScene();

	void ProcessInput() override;
	void Update() override;
	void GenerateOutput() override;

private:
};

#pragma once
#include "BaseScene.h"

class ResultScene : public BaseScene
{
public:
	ResultScene(class SceneManager* sm, const class Parameter& parameter);
	~ResultScene();

	void ProcessInput() override;
	void Update() override;
	void GenerateOutput() override;

private:
};

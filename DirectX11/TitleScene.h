#pragma once
#include "BaseScene.h"

class TitleScene : public BaseScene
{
public:
	TitleScene(class SceneManager* sm, const class Parameter& parameter);
	~TitleScene();

	void ProcessInput() override;
	void Update() override;
	void GenerateOutput() override;

private:
};

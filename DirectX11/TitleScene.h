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
	bool mIsTutorial;

	class SpriteComponent* mTitleSprite;
	class SpriteComponent* mModeSprite;
	class SpriteComponent* mBackSprite;
	class SpriteComponent* mTutorialSprite;
};

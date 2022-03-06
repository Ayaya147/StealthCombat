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
	float mAlpha;
	float mRate;

	class SpriteComponent* mTitle1Sprite;
	class SpriteComponent* mTitle2Sprite;
	class SpriteComponent* mModeSprite;
	class SpriteComponent* mBackSprite;
	class SpriteComponent* mTutorialSprite;
};

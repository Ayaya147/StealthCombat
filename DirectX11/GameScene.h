#pragma once
#include "BaseScene.h"

class GameScene : public BaseScene
{
public:
	GameScene(class SceneManager* sm, const class Parameter& parameter);
	~GameScene();

	void ProcessInput() override;
	void Update() override;
	void GenerateOutput() override;

	class CloudActor* GetCloud() const { return mCloud; }
	class PlaneActor* GetPlane() const { return mPlane; }

private:
	class CloudActor* mCloud;
	class PlaneActor* mPlane;
	class SpriteComponent* mSprite;
};

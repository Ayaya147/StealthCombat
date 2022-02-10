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

	class PhysWorld* GetPhysWorld() const { return mPhysWorld; }
	class CloudActor* GetCloud() const { return mCloud; }
	class PlaneActor* GetPlane() const { return mPlane; }
	class PlayerActor* GetPlayer() const { return mPlayer; }

private:
	class PhysWorld* mPhysWorld;
	class PlayerActor* mPlayer;
	class CloudActor* mCloud;
	class PlaneActor* mPlane;
	class SpriteComponent* mSprite;
};

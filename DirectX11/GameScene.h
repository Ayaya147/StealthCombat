#pragma once
#include "BaseScene.h"
#include <vector>

class GameScene : public BaseScene
{
public:
	GameScene(class SceneManager* sm, const class Parameter& parameter);
	~GameScene();

	void ProcessInput() override;
	void Update() override;
	void GenerateOutput() override;

	void AddEnemy(class EnemyActor* enemy);
	void RemoveEnemy(class EnemyActor* enemy);

	const std::vector<class EnemyActor*>& GetEnemies() const { return mEnemies; }
	class PhysWorld* GetPhysWorld() const { return mPhysWorld; }
	class CloudActor* GetCloud() const { return mCloud; }
	class PlaneActor* GetPlane() const { return mPlane; }
	class PlayerActor* GetPlayer() const { return mPlayer; }

private:
	std::vector<class EnemyActor*> mEnemies;

	class PhysWorld* mPhysWorld;
	class PlayerActor* mPlayer;
	class CloudActor* mCloud;
	class PlaneActor* mPlane;
	class Actor* mDirectionMinimap;
	class SpriteComponent* mSprite;
};

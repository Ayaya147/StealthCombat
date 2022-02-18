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
	void AddCloud(class CloudActor* cloud);
	void RemoveCloud(class CloudActor* cloud);

	const std::vector<class EnemyActor*>& GetEnemies() const { return mEnemies; }
	const std::vector<class CloudActor*>& GetClouds() const { return mClouds; }
	class PhysWorld* GetPhysWorld() const { return mPhysWorld; }
	class Minimap* GetMinimap() const { return mMap; }
	class CloudActor* GetCloud() const { return mCloud; }
	class PlaneActor* GetPlane() const { return mPlane; }
	class PlayerActor* GetPlayer() const { return mPlayer; }
	class SpriteComponent* GetMarkingSprite() const { return mMarkingSprite; }

private:
	std::vector<class EnemyActor*> mEnemies;
	std::vector<class CloudActor*> mClouds;

	class PhysWorld* mPhysWorld;
	class PlayerActor* mPlayer;
	class CloudActor* mCloud;
	class PlaneActor* mPlane;
	class NumberActor* mCloudTimeNum;
	class NumberActor* mSpdNum;
	class NumberActor* mEnemyNum;
	class SpriteComponent* mGuideSprite;
	class SpriteComponent* mMarkingSprite;
	class Minimap* mMap;
};

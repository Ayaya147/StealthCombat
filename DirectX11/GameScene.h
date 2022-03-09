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

	void SetDestroyedSpriteTime(float time) { mDestroyedSpriteTime = time; }
	void SetVibrationStrength(int strength) { mVibrationStrength = strength; }
	const std::vector<class EnemyActor*>& GetEnemies() const { return mEnemies; }
	const std::vector<class CloudActor*>& GetClouds() const { return mClouds; }
	class PhysWorld* GetPhysWorld() const { return mPhysWorld; }
	class Minimap* GetMinimap() const { return mMap; }
	class PlayerActor* GetPlayer() const { return mPlayer; }
	class SpriteComponent* GetMarkingEnemySprite() const { return mMarkingEnemySprite; }
	class SpriteComponent* GetMarkingPlayerSprite() const { return mMarkingPlayerSprite; }
	int GetAirplaneBGM() const { return mAirplaneBGM; }

private:
	std::vector<class EnemyActor*> mEnemies;
	std::vector<class CloudActor*> mClouds;
	class Minimap* mMap;
	class PhysWorld* mPhysWorld;
	class PlayerActor* mPlayer;
	class NumberActor* mOutCloudTime;
	class NumberActor* mSpdNum;
	class NumberActor* mEnemyNum;
	class NumberActor* mRestTime;
	class SpriteComponent* mGuideSprite;
	class SpriteComponent* mMarkingEnemySprite;
	class SpriteComponent* mMarkingPlayerSprite;
	class SpriteComponent* mCautionGameTime;
	class SpriteComponent* mCautionCloudTime;
	class SpriteComponent* mTimeSprite;
	class SpriteComponent* mUICountSprite;
	class SpriteComponent* mDestroyedSprite;
	class UIScreen* mBackgroundUI;
	class UIScreen* mMenuUI;
	class UIScreen* mVictoryUI;
	class UIScreen* mDefeatUI;

	bool mWin;
	float mQuitTime;
	float mDestroyedSpriteTime;
	int mAirplaneBGM;
	int mGameBGM;
	int mVibrationStrength;
};

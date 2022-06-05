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
	void RemoveEnemyGuideTranslucenceComp();

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
	void CreateGameActor();
	void CreateNumberActor();
	void CreateUIActor();
	void CreatePauseScreen();
	void CreateSound();

	std::vector<class EnemyActor*> mEnemies;
	std::vector<class CloudActor*> mClouds;
	std::vector<class TranslucenceComponent*> mEnemyGuideTranslucenceComps;
	class Minimap* mMap;
	class PhysWorld* mPhysWorld;
	class PlayerActor* mPlayer;
	class NumberActor* mOutCloudTime;
	class NumberActor* mSpdNum;
	class NumberActor* mEnemyNum;
	class NumberActor* mRestTime;
	class NumberActor* mFPS;
	class SpriteComponent* mGuideSprite;
	class SpriteComponent* mMarkingEnemySprite;
	class SpriteComponent* mMarkingPlayerSprite;
	class SpriteComponent* mCautionGameTime;
	class SpriteComponent* mCautionCloudTime;
	class SpriteComponent* mTimeSprite;
	class SpriteComponent* mUICountSprite;
	class SpriteComponent* mDestroyedSprite;
	class PauseScreen* mBackgroundUI;
	class PauseScreen* mMenuUI;

	bool mIsGameWin;
	bool mIsCautionSE;
	float mQuitTime;
	float mDestroyedSpriteTime;
	int mAirplaneBGM;
	int mGameBGM;
	int mCautionSE;
	int mLockOnSE;
	int mVibrationStrength;
};

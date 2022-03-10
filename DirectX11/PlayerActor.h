#pragma once
#include "Actor.h"
#include <DirectXMath.h>

class PlayerActor : public Actor
{
public:
	PlayerActor(class BaseScene* scene);
	
	void ActorInput() override;
	void UpdateActor(float deltaTime) override;

	void SetIsLockOnSE(bool is) { mIsLockOnSE = is; }
	void SetLockedOn(bool lockon) { mIsLockedOn = lockon; }
	bool GetIsLockedOn() const { return mIsLockedOn; }
	float GetForwardSpeed() const;
	float GetOutCloudTime() const { return mOutCloudTime; }
	bool GetIsInCloud() const { return mIsInCloud; }
	class SphereComponent* GetSphereComp() const { return mBody; }
	class CameraComponent* GetCameraComp() const { return mCameraComponent; }

private:
	class CameraComponent* mCameraComponent;
	class MoveComponent* mMoveComponent;
	class SphereComponent* mBody;
	class SphereComponent* mAttackRange;
	class SpriteComponent* mPlayerSprite;
	class EnemyActor* mTargetEnemy;
	bool mIsLockedOn;
	bool mIsInCloud;
	bool mIsLockOnSE;
	float mOutCloudTime;
	float mEmitterCD;
	float mOutCloudTimeMax;
};

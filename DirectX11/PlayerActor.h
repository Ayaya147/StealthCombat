#pragma once
#include "Actor.h"

class PlayerActor : public Actor
{
public:
	PlayerActor(class BaseScene* scene);
	
	void ActorInput() override;
	void UpdateActor(float deltaTime) override;

	void SetLockedOn(bool lockon) { mIsLockedOn = lockon; }
	bool GetIsLockedOn() const { return mIsLockedOn; }
	float GetForwardSpeed() const;
	float GetOutCloudTime() const { return mOutCloudTime; }
	class SphereComponent* GetSphereComp() const { return mBody; }

private:
	class MoveComponent* mMoveComponent;
	class SphereComponent* mBody;
	class SphereComponent* mAttackRange;
	class EnemyActor* mTargetEnemy;
	bool mIsLockedOn;
	float mOutCloudTime;
};

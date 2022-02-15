#pragma once
#include "Actor.h"

class EnemyActor : public Actor
{
public:
	EnemyActor(class BaseScene* scene);
	~EnemyActor();

	void UpdateActor(float deltaTime) override;
	void CalcDistFromPlayer();

	void SetLockedOn(bool is) { mIsLockedOn = is; }
	bool GetIsLockedOn() const { return mIsLockedOn; }
	float GetDistFromPlayer() const { return mDist; }
	class SphereComponent* GetSphereComp() const { return mSphereComponent; }

private:
	class MoveComponent* mMoveComponent;
	class SphereComponent* mSphereComponent;
	float mDist;
	bool mIsLockedOn;
};

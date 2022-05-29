#pragma once
#include "Actor.h"

class EnemyActor : public Actor
{
public:
	EnemyActor(class BaseScene* scene);
	~EnemyActor();

	void UpdateActor(float deltaTime) override;

	void SetLockedOn(bool lockon) { mIsLockedOn = lockon; }
	bool GetIsLockedOn() const { return mIsLockedOn; }
	bool GetIsInCloud() const { return mIsInCloud; }
	class SphereComponent* GetBodySphereComp() const { return mBody; }

private:
	void CalcNextDestination();
	float CalcAngle();

	class MoveComponent* mMoveComponent;
	class SphereComponent* mBody;
	class SphereComponent* mAttackRange;
	bool mIsLockedOn;
	bool mIsInCloud;
	float mTimeChangeDestination;
	float mSign;
	float mEmitterCD;
	DirectX::XMFLOAT3 mTargetLocationPos;
};

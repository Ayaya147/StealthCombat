#pragma once
#include "Actor.h"
#include <DirectXMath.h>

class EnemyActor : public Actor
{
public:
	EnemyActor(class BaseScene* scene);
	~EnemyActor();

	void UpdateActor(float deltaTime) override;

	void SetLockedOn(bool lockon) { mIsLockedOn = lockon; }
	bool GetIsLockedOn() const { return mIsLockedOn; }
	bool GetIsInCloud() const { return mIsInCloud; }
	float GetDistFromPlayer() const { return mDist; }
	class SphereComponent* GetSphereComp() const { return mBody; }

private:
	void CalcNextPos();
	float CalcAngle();

	class MoveComponent* mMoveComponent;
	class SphereComponent* mBody;
	class SphereComponent* mAttackRange;
	float mDist;
	bool mIsLockedOn;
	bool mIsInCloud;
	float mTime;
	float mSign;
	DirectX::XMFLOAT3 mTargetLocationPos;
};

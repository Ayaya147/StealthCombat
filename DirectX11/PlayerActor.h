#pragma once
#include "Actor.h"
#include <DirectXMath.h>

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
	DirectX::XMFLOAT3 LocalToClip(class Actor* actor);

	class MoveComponent* mMoveComponent;
	class SphereComponent* mBody;
	class SphereComponent* mAttackRange;
	class SpriteComponent* mPlayerSprite;
	class EnemyActor* mTargetEnemy;
	bool mIsLockedOn;
	float mOutCloudTime;
	float mEmitterCD;
};

#pragma once
#include "Actor.h"

class PlayerActor : public Actor
{
public:
	PlayerActor(class BaseScene* scene);

	void UpdateActor(float deltaTime) override;
	void ActorInput() override;

	float GetForwardSpeed() const;
	float GetOutCloudTime() const { return mOutCloudTime; }

private:
	class MoveComponent* mMoveComponent;
	class SphereComponent* mBody;
	class SphereComponent* mAttackRange;
	float mOutCloudTime;
};

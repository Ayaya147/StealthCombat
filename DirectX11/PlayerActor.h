#pragma once
#include "Actor.h"

class PlayerActor : public Actor
{
public:
	PlayerActor(class BaseScene* scene);

	void UpdateActor(float deltaTime) override;
	void ActorInput() override;

	float GetForwardSpeed() const;

private:
	class MoveComponent* mMoveComponent;
	class SphereComponent* mBody;
	class SphereComponent* mAttackRange;
	class NumberActor* mCloudTimeNum;
	class NumberActor* mSpdNum;
	float mOutCloudTime;
};

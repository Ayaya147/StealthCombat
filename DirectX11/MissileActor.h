#pragma once
#include "Actor.h"

class MissileActor : public Actor
{
public:
	MissileActor(class BaseScene* scene);

	void UpdateActor(float deltaTime) override;

private:
	class MoveComponent* mMoveComponent;
};

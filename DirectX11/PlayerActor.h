#pragma once
#include "Actor.h"

class PlayerActor : public Actor
{
public:
	PlayerActor(class BaseScene* scene);

	void UpdateActor(float deltaTime) override;
	void ActorInput() override;

private:
	class MoveComponent* mMoveComponent;
};

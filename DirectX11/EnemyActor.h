#pragma once
#include "Actor.h"

class EnemyActor : public Actor
{
public:
	EnemyActor(class BaseScene* scene);

	void UpdateActor(float deltaTime) override;

private:
	class MoveComponent* mMoveComponent;
};

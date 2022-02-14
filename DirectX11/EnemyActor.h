#pragma once
#include "Actor.h"

class EnemyActor : public Actor
{
public:
	EnemyActor(class BaseScene* scene);
	~EnemyActor();

	void UpdateActor(float deltaTime) override;
	void CalcDistFromPlayer();

	float GetDistFromPlayer() const { return mDist; }

private:
	class MoveComponent* mMoveComponent;
	float mDist;
};

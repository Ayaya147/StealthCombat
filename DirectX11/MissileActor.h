#pragma once
#include "Actor.h"
#include <DirectXMath.h>

class MissileActor : public Actor
{
public:
	enum class MissileType
	{
		ETargetEnemy,
		ETargetPlayer
	};

	MissileActor(class BaseScene* scene, class Actor* actor, const DirectX::XMFLOAT3& pos, float forwardSpeed);

	void UpdateActor(float deltaTime) override;

private:
	class MoveComponent* mMoveComponent;
	class SphereComponent* mSphereComponent;
	class Actor* mTarget;
	MissileType mType;
	float mSmokeCD;
};

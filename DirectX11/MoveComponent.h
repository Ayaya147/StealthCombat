#pragma once
#include "Component.h"

class MoveComponent : public Component
{
public:
	enum class MoveType
	{
		EStraight,
		ECornering
	};

	MoveComponent(class Actor* owner, int updateOrder = 10);

	void Update(float deltaTime) override;

	float GetAngularSpeed() const { return mAngularSpeed; }
	float GetForwardSpeed() const { return mForwardSpeed; }
	void SetMoveType(MoveType type) { mType = type; }
	void SetForwardSpeedMax(float max) { mForwardSpeedMax = max; }
	void SetForwardSpeedMin(float min) { mForwardSpeedMin = min; }
	void SetAngularSpeed(float speed) { mAngularSpeed = speed; }
	void SetForwardSpeed(float speed) { mForwardSpeed = speed; }
	void SetAcceleration(float accel) { mAcceleration = accel; }

private:
	MoveType mType;
	float mForwardSpeedMax;
	float mForwardSpeedMin;
	float mAngularSpeed;
	float mForwardSpeed;
	float mAcceleration;
};

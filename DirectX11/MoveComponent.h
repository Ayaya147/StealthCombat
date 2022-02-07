#pragma once
#include "Component.h"

class MoveComponent : public Component
{
public:
	MoveComponent(class Actor* owner, int updateOrder = 10);
	void Update(float deltaTime) override;

	float GetAngularSpeed() const { return mAngularSpeed; }
	float GetForwardSpeed() const { return mForwardSpeed; }
	void SetForwardSpeedMax(float max) { mForwardSpeedMax = max; }
	void SetAngularSpeed(float speed) { mAngularSpeed = speed; }
	void SetAcceleration(float accel) { mAcceleration = accel; }

private:
	float mForwardSpeedMax;
	float mAngularSpeed;
	float mForwardSpeed;
	float mAcceleration;
};

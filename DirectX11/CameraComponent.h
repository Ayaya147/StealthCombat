#pragma once
#include "Component.h"
#include <DirectXMath.h>

class CameraComponent : public Component
{
public:
	CameraComponent(class Actor* owner, int updateOrder = 200);

	void Update(float deltaTime) override;
	void ProcessInput() override;

private:
	float mTargetDist;
	int mType;
};

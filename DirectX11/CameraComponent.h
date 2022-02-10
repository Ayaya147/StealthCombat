#pragma once
#include "Component.h"
#include <DirectXMath.h>

class CameraComponent : public Component
{
public:
	CameraComponent(class Actor* owner, int updateOrder = 200);

	void Update(float deltaTime) override;
	void ProcessInput() override;

	DirectX::XMFLOAT3 ComputeCameraPos();
	void SnapToIdeal();

private:
	float mTargetDist;
	int mType;
	float mSpringConstant;
	DirectX::XMFLOAT3 mVelocity;
	DirectX::XMFLOAT3 mActualPos;
};

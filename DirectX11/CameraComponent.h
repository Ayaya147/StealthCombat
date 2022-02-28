#pragma once
#include "Component.h"
#include <DirectXMath.h>

class CameraComponent : public Component
{
public:
	CameraComponent(class Actor* owner, int updateOrder = 200);

	void ProcessInput() override;
	void Update(float deltaTime) override;

	DirectX::XMFLOAT3 ComputeCameraPos();
	void SnapToIdeal();

private:
	float mTargetDist;
	DirectX::XMFLOAT3 mVelocity;
	DirectX::XMFLOAT3 mActualPos;
};

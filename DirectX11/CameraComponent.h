#pragma once
#include "Component.h"
#include <DirectXMath.h>

class CameraComponent : public Component
{
public:
	enum class CameraState
	{
		ENormal,
		EAccel,
		EExplosion
	};

	CameraComponent(class Actor* owner, int updateOrder = 200);

	void ProcessInput() override;
	void Update(float deltaTime) override;
	DirectX::XMFLOAT3 ComputeCameraPos();
	void SnapToIdeal();

	void SetCameraState(CameraState state) { mState = state; }
	CameraState GetCameraState() const { return mState; }

private:
	CameraState mState;
	float mTargetDist;
	DirectX::XMFLOAT3 mVelocity;
	DirectX::XMFLOAT3 mActualPos;
};

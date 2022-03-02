#pragma once
#include "Component.h"
#include <DirectXMath.h>

class CameraComponent : public Component
{
public:
	enum class VibrationState
	{
		ENone,
		ENormal,
		EHard
	};

	CameraComponent(class Actor* owner, int updateOrder = 200);

	void ProcessInput() override;
	void Update(float deltaTime) override;
	DirectX::XMFLOAT3 ComputeCameraPos();
	void SnapToIdeal();

	void SetCameraState(VibrationState state) { mState = state; }
	VibrationState GetCameraState() const { return mState; }

private:
	VibrationState mState;
	float mTargetDist;
	DirectX::XMFLOAT3 mVelocity;
	DirectX::XMFLOAT3 mActualPos;
};

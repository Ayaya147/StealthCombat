#pragma once
#include "Component.h"
#include <DirectXMath.h>

class CameraComponent : public Component
{
public:
	enum class CameraType
	{
		ENormal,
		ESpring
	};

	enum class VibrationState
	{
		ENone,
		ELight,
		ENormal,
		EHard
	};

	CameraComponent(class Actor* owner, CameraType type, int updateOrder = 200);

	void ProcessInput() override;
	void Update(float deltaTime) override;
	void SnapToIdeal();

	void SetCameraState(VibrationState state) { mState = state; }
	VibrationState GetCameraState() const { return mState; }

private:
	DirectX::XMFLOAT3 ComputeCameraPos();

	CameraType mType;
	VibrationState mState;
	float mTargetDist;
	DirectX::XMFLOAT3 mVelocity;
	DirectX::XMFLOAT3 mActualPos;
	DirectX::XMFLOAT3 mUp;
};

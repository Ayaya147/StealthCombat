#pragma once
#include "Actor.h"

template<class C>
class PixelConstantBuffer;

class SmokeActor : public Actor
{
public:
	SmokeActor(class BaseScene* scene, DirectX::XMFLOAT3 rotation = {});
	~SmokeActor();

	void UpdateActor(float deltaTime) override;
	void Bind(class Renderer* renderer) override;
	void ImGuiWindow();

private:
	struct ObjectConstant
	{
		DirectX::XMMATRIX mWorldTransformInverse;
		float mTime;
		int mType;
		float padding[2];
	};

	struct SmokeConstant
	{
		alignas(16) DirectX::XMFLOAT3 mColor;
		float mLoop;
		float mNoiseOffset;
		float mNoiseScale;
		float mRadius;
		float mAbsorption;
		float mOpacity;
		float mAbsorptionLight;
		float mOpacityLight;
		float mLightStepScale;
		int mLoopLight;
		float padding[3];
	};

	void Reset();

	float mAnimationSpeed;
	SmokeConstant mData;
	bool mIsAnimation;
	DirectX::XMFLOAT3 mMissileRotation;
	static int mCount;
	static PixelConstantBuffer<ObjectConstant>* mObjectCBuffer;
	static PixelConstantBuffer<SmokeConstant>* mSmokeCBuffer;
};

#pragma once
#include "Actor.h"

template<class C>
class PixelConstantBuffer;

class CloudActor : public Actor
{
public:
	CloudActor(class BaseScene* scene);
	~CloudActor();

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

	struct CloudConstant
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
		float padding[2];
	};

	void Reset();

	float mAnimationSpeed;
	bool mIsAnimation;
	CloudConstant mData;
	static int mCount;
	static PixelConstantBuffer<ObjectConstant>* mObjectCBuffer;
	static PixelConstantBuffer<CloudConstant>* mCloudCBuffer;
};

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
	float CalcDistFromCamera();
	void ImGuiWindow();

	float GetDistFromCamera() const { return mDistFromCamera; }

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

	float mDistFromCamera;
	static int mCount;
	bool mIsAnimation;
	static CloudConstant mData;
	static PixelConstantBuffer<ObjectConstant>* mObjectCBuffer;
	static PixelConstantBuffer<CloudConstant>* mCloudCBuffer;
};

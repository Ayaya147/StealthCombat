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
	void Reset();

	float GetDistFromCamera() const { return mDistFromCamera; }

private:
	struct ObjectConstant
	{
		DirectX::XMMATRIX mWorldTransformInverse;
		float mTime;
		float padding[3];
	};

	struct CloudConstant
	{
		alignas(16) DirectX::XMFLOAT3 mCloudColor;
		int mLoop;
		float mNoiseScale;
		float mRadius;
		int mAbsorption;
		int mOpacity;
		int mAbsorptionLight;
		int mOpacityLight;
		float mLightStepScale;
		int mLoopLight;
		float padding[3];
	};

	float mDistFromCamera;
	static int mCount;
	static CloudConstant mData;
	static PixelConstantBuffer<ObjectConstant>* mObjectCBuffer;
	static PixelConstantBuffer<CloudConstant>* mCloudCBuffer;
};

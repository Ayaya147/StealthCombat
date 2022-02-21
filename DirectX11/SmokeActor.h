#pragma once
#include "Actor.h"

template<class C>
class PixelConstantBuffer;

class SmokeActor : public Actor
{
public:
	SmokeActor(class BaseScene* scene);
	~SmokeActor();

	void UpdateActor(float deltaTime) override;
	void Bind(class Renderer* renderer) override;
	void ImGuiWindow();
	void Reset();

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

	static int mCount;
	SmokeConstant mData;
	static PixelConstantBuffer<ObjectConstant>* mObjectCBuffer;
	static PixelConstantBuffer<SmokeConstant>* mSmokeCBuffer;
};

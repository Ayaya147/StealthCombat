#pragma once
#include <string>
#include "Actor.h"

template<class C>
class PixelConstantBuffer;

class CloudActor : public Actor
{
public:
	CloudActor(class BaseScene* scene);
	~CloudActor();

	void UpdateActor(float deltaTime) override {};
	void ActorInput() override {};
	void Bind(class Renderer* renderer) override;
	void ImGuiWinodow();
	void Reset();

private:
	struct ObjectConstant
	{
		DirectX::XMMATRIX mWorldTransformInverse;
	};

	struct CloudConstant
	{
		alignas(16) DirectX::XMFLOAT3 mCloudColor;
		int mLoop;
		int mNoiseScale;
		float mRadius;
		int mAbsorption;
		int mOpacity;
		int mAbsorptionLight;
		int mOpacityLight;
		float mLightStepScale;
		int mLoopLight;
		float padding[3];
	};

	CloudConstant mData;
	static int mCount;
	static PixelConstantBuffer<ObjectConstant>* mObjectCBuffer;
	static PixelConstantBuffer<CloudConstant>* mCloudCBuffer;
};

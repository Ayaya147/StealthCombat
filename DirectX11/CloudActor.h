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

	void UpdateActor(float deltaTime) override;
	void Bind(class Renderer* renderer) override;
	float CalculateZValue();
	void ImGuiWinodow();
	void Reset();

	float GetZValue() const { return mZValue; }

private:
	struct ObjectConstant
	{
		DirectX::XMMATRIX mWorldTransformInverse;
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
		float mTime;
		float padding[2];
	};

	CloudConstant mData;
	float mZValue;
	static int mCount;
	static PixelConstantBuffer<ObjectConstant>* mObjectCBuffer;
	static PixelConstantBuffer<CloudConstant>* mCloudCBuffer;
};

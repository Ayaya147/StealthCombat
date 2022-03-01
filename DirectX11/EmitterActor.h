#pragma once
#include "Actor.h"

template<class C>
class PixelConstantBuffer;

class EmitterActor : public Actor
{
public:
	EmitterActor(class BaseScene* scene);
	~EmitterActor();

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

	struct EmitterConstant
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

	static int mCount;
	EmitterConstant mData;
	bool mIsAnimation;
	static PixelConstantBuffer<ObjectConstant>* mObjectCBuffer;
	static PixelConstantBuffer<EmitterConstant>* mEmitterCBuffer;
};

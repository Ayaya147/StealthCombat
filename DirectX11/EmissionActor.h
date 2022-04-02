#pragma once
#include "Actor.h"

template<class C>
class PixelConstantBuffer;

class EmissionActor : public Actor
{
public:
	EmissionActor(class BaseScene* scene);
	~EmissionActor();

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

	struct EmissionConstant
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

	float mAnimationSpeed;
	EmissionConstant mData;
	bool mIsAnimation;
	static int mCount;
	static PixelConstantBuffer<ObjectConstant>* mObjectCBuffer;
	static PixelConstantBuffer<EmissionConstant>* mEmissionCBuffer;
};

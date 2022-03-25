#pragma once
#include "Actor.h"

template<class C>
class PixelConstantBuffer;

class ExplosionActor : public Actor
{
public:
	ExplosionActor(class BaseScene* scene);
	~ExplosionActor();

	void UpdateActor(float deltaTime) override;
	void Bind(class Renderer* renderer) override;
	void ImGuiWindow();

private:
	enum class ExplosionPhase
	{
		EOne,
		ETwo
	};

	struct ObjectConstant
	{
		DirectX::XMMATRIX mWorldTransformInverse;
		float mTime;
		int mType;
		float padding[2];
	};

	struct ExplosionConstant
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
	float mSpeed;
	bool mIsAnimation;
	ExplosionConstant mData;
	ExplosionPhase mPhase;
	static PixelConstantBuffer<ObjectConstant>* mObjectCBuffer;
	static PixelConstantBuffer<ExplosionConstant>* mExplosionCBuffer;
};

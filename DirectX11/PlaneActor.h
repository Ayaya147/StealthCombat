#pragma once
#include "Actor.h"

template<class C>
class VertexConstantBuffer;

template<class C>
class PixelConstantBuffer;

class PlaneActor : public Actor
{
public:
	PlaneActor(class BaseScene* scene);
	~PlaneActor();

	void UpdateActor(float deltaTime) override;
	void Bind(class Renderer* renderer) override;
	void ImGuiWindow();

private:
	struct VertexConstant
	{
		float mTime;
		float mNoiseStrength;
		float mNoiseSizeLerp;
		float mWaveSpeed;
	};

	struct PixelConstant
	{
		alignas(16) DirectX::XMFLOAT3 mSeaBaseColor;
		alignas(16) DirectX::XMFLOAT3 mSeaShallowColor;
		alignas(16) DirectX::XMFLOAT3 mSkyColor;
		float mTime;
		float mNoiseStrength;
		float mNoiseSizeLerp;
		float mWaveSpeed;
		float mBaseColorStrength;
		float mShallowColorStrength;
		float mShininess;
		float mColorHeightOffset;
		float mF0;
		float padding[3];
	};

	void Reset();

	VertexConstant mVertexData;
	PixelConstant mPixelData;
	VertexConstantBuffer<VertexConstant>* mVertexCBuffer;
	PixelConstantBuffer<PixelConstant>* mPixelCBuffer;
	class VertexBuffer* mVertexBuffer;
	int mCount;
};

#pragma once
#include "Renderer.h"

struct DirectionalLight
{
	DirectX::XMFLOAT3 mDirection;
	DirectX::XMFLOAT3 mAmbientColor;
	DirectX::XMFLOAT3 mDiffuseColor;
	DirectX::XMFLOAT3 mSpecColor;
};

template<class C>
class PixelConstantBuffer;

class Light
{
public:
	Light(class Renderer* renderer);
	~Light();

	void Bind(class Renderer* renderer);
	void Reset();
	void ImGuiWindow();

	void SetDirectionalLight(const DirectionalLight& light);

private:
	struct PixelConstant
	{
		alignas(16) DirectX::XMFLOAT3 mCameraPos;
		alignas(16) DirectX::XMFLOAT3 mDirection;
		alignas(16) DirectX::XMFLOAT3 mAmbientColor;
		alignas(16) DirectX::XMFLOAT3 mDiffuseColor;
		alignas(16) DirectX::XMFLOAT3 mSpecColor;
	};

	PixelConstant mData;
	PixelConstantBuffer<PixelConstant>* mPixelCBuffer;
};

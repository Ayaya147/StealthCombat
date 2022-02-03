#pragma once
#include "Renderer.h"

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

	void SetDirectionalLight(const DirectX::XMFLOAT3& dir, const DirectX::XMFLOAT3& ambient, const DirectX::XMFLOAT3& diff, const DirectX::XMFLOAT3& spec);

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

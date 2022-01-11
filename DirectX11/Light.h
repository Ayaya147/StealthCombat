#pragma once
#include "Renderer.h"

template<class C>
class PixelConstantBuffer;

class Light
{
public:
	Light(Renderer* renderer);
	~Light();

	void Bind(Renderer* renderer);

private:
	struct ObjectConstant
	{
		alignas(16) DirectX::XMFLOAT3 cameraPos;
		alignas(16) DirectX::XMFLOAT3 ambientLight;
		float specPower;
		float padding[3];
	};

	struct DirectionalLightConstant
	{
		alignas(16) DirectX::XMFLOAT3 mDirection;
		alignas(16) DirectX::XMFLOAT3 mDiffuseColor;
		alignas(16) DirectX::XMFLOAT3 mSpecColor;
	};

	DirectX::XMFLOAT3 mAmbientLight;
	DirectionalLightConstant mDirLight;

	PixelConstantBuffer<ObjectConstant>* mObjectCBuffer;
	PixelConstantBuffer<DirectionalLightConstant>* mDLightCBuffer;
};

#pragma once
#include "Renderer.h"

template<class C>
class PixelConstantBuffer;

struct DirectionalLightConstant
{
	alignas(16) DirectX::XMFLOAT3 mDirection;
	alignas(16) DirectX::XMFLOAT3 mDiffuseColor;
	alignas(16) DirectX::XMFLOAT3 mSpecColor;
};

class Light
{
public:
	Light(class Renderer* renderer);
	~Light();

	void Bind(class Renderer* renderer);

	void SetAmbientLight(const DirectX::XMFLOAT3& ambient) { mAmbientLight = ambient; }
	void SetDirectionalLight(const DirectionalLightConstant& direct) { mDirLight = direct; }

private:
	struct ObjectConstant
	{
		alignas(16) DirectX::XMFLOAT3 mCameraPos;
		alignas(16) DirectX::XMFLOAT3 mAmbientLight;
	};

	DirectX::XMFLOAT3 mAmbientLight;
	DirectionalLightConstant mDirLight;

	PixelConstantBuffer<ObjectConstant>* mObjectCBuffer;
	PixelConstantBuffer<DirectionalLightConstant>* mDLightCBuffer;
};

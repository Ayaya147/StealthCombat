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

	void SetAmbientLight(const DirectX::XMFLOAT3& ambient);
	void SetDirectionalLight(const DirectX::XMFLOAT3& dir, const DirectX::XMFLOAT3& diff, const DirectX::XMFLOAT3& spec);

private:
	struct ObjectConstant
	{
		alignas(16) DirectX::XMFLOAT3 mCameraPos;
		alignas(16) DirectX::XMFLOAT3 mAmbientLight;
		alignas(16) DirectX::XMFLOAT3 mDirection;
		alignas(16) DirectX::XMFLOAT3 mDiffuseColor;
		alignas(16) DirectX::XMFLOAT3 mSpecColor;
	};

	ObjectConstant mObjectConstant;
	PixelConstantBuffer<ObjectConstant>* mObjectCBuffer;
};

#include "Light.h"
#include "ConstantBuffer.h"

namespace dx = DirectX;

Light::Light(Renderer* renderer)
{
	mObjectCBuffer = new PixelConstantBuffer<ObjectConstant>(renderer, 0);
}

Light::~Light()
{
	delete mObjectCBuffer;
}

void Light::Bind(Renderer* renderer)
{
	dx::XMMATRIX invView = dx::XMMatrixInverse(nullptr, renderer->GetViewMatrix());
	dx::XMStoreFloat3(&mObjectConstant.mCameraPos, invView.r[3]);

	mObjectCBuffer->Update(renderer, mObjectConstant);
	mObjectCBuffer->Bind(renderer);
}

void Light::SetAmbientLight(const DirectX::XMFLOAT3& ambient)
{
	mObjectConstant.mAmbientLight = ambient;
}

void Light::SetDirectionalLight(const DirectX::XMFLOAT3& dir, const DirectX::XMFLOAT3& diff, const DirectX::XMFLOAT3& spec)
{
	mObjectConstant.mDirection = dir;
	mObjectConstant.mDiffuseColor = diff;
	mObjectConstant.mSpecColor = spec;
}

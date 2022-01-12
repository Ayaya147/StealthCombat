#include "Light.h"
#include "ConstantBuffer.h"

namespace dx = DirectX;

Light::Light(Renderer* renderer)
{
	mObjectCBuffer = new PixelConstantBuffer<ObjectConstant>(renderer, 0);
	mDLightCBuffer = new PixelConstantBuffer<DirectionalLightConstant>(renderer, 1);

	mAmbientLight = dx::XMFLOAT3{ 0.4f, 0.4f, 0.4f };
	mDirLight.mDirection = dx::XMFLOAT3{ 0.0f, -1.0f, -1.0f };
	mDirLight.mDiffuseColor = dx::XMFLOAT3{ 0.8f, 0.9f, 1.0f };
	mDirLight.mSpecColor = dx::XMFLOAT3{ 0.8f, 0.8f, 0.8f };
}

Light::~Light()
{
	delete mObjectCBuffer;
	delete mDLightCBuffer;
}

void Light::Bind(Renderer* renderer)
{
	ObjectConstant c = {};
	c.ambientLight = mAmbientLight;
	dx::XMMATRIX invView = dx::XMMatrixInverse(nullptr, renderer->GetViewMatrix());
	dx::XMStoreFloat3(&c.cameraPos, invView.r[3]);

	mObjectCBuffer->Update(renderer, c);
	mObjectCBuffer->Bind(renderer);

	mDLightCBuffer->Update(renderer, mDirLight);
	mDLightCBuffer->Bind(renderer);
}

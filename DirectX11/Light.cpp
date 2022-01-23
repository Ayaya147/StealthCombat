#include "Light.h"
#include "ConstantBuffer.h"

namespace dx = DirectX;

Light::Light(Renderer* renderer)
{
	mObjectCBuffer = new PixelConstantBuffer<ObjectConstant>(renderer, 0);
	mDLightCBuffer = new PixelConstantBuffer<DirectionalLightConstant>(renderer, 1);
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

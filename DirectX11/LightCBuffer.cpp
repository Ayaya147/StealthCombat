#include "LightCBuffer.h"
#include "ConstantBuffer.h"

namespace dx = DirectX;

LightCBuffer::LightCBuffer(Renderer* renderer)
{
	mCBuffer = new PixelConstantBuffer<Constant>(renderer, 0);
	mLightBuffer = new PixelConstantBuffer<DirectionalLight>(renderer, 1);
}

LightCBuffer::~LightCBuffer()
{
	delete mCBuffer;
	delete mLightBuffer;
}

void LightCBuffer::Bind(Renderer* renderer)
{
	Constant c = {};
	c.ambientLight = renderer->GetAmbientLight();
	dx::XMMATRIX invView = dx::XMMatrixInverse(nullptr, renderer->GetViewMatrix());
	dx::XMStoreFloat3(&c.cameraPos, invView.r[3]);
	c.specPower = 20;

	mCBuffer->Update(renderer, c);
	mCBuffer->Bind(renderer);

	DirectionalLight light = renderer->GetDirectionalLight();
	mLightBuffer->Update(renderer, light);
	mLightBuffer->Bind(renderer);
}

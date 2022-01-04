#include "TransformCBufferDouble.h"
#include "Renderer.h"
#include "Actor.h"
#include "ConstantBuffer.h"

namespace dx = DirectX;

PixelConstantBuffer<TransformCBuffer::Transforms>* TransformCBufferDouble::mPBuffer = nullptr;

TransformCBufferDouble::TransformCBufferDouble(Renderer* renderer, Actor* actor, UINT slot)
	:
	TransformCBuffer(renderer,actor,0)
{
	if (!mPBuffer)
	{
		mPBuffer = new PixelConstantBuffer<Transforms>(renderer, slot);
	}
}

TransformCBufferDouble::~TransformCBufferDouble()
{
	delete mPBuffer;
}

void TransformCBufferDouble::Bind(Renderer* renderer)
{
	TransformCBuffer::Bind(renderer);

	dx::XMMATRIX view = renderer->GetViewMatrix();
	dx::XMMATRIX projection = renderer->GetProjectionMatrix();
	dx::XMMATRIX worldTransform = mOwner->GetWorldTransform();

	const Transforms tf =
	{
		dx::XMMatrixTranspose(worldTransform),
		dx::XMMatrixTranspose(view * projection)
	};

	mPBuffer->Update(renderer, tf);
	mPBuffer->Bind(renderer);
}

void TransformCBufferDouble::UpdateBindImpl(Renderer* renderer, const Transforms& tf) noexcept
{
}

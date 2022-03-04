#include "TransformCBuffer.h"
#include "ConstantBuffer.h"
#include "Renderer.h"
#include "Actor.h"

namespace dx = DirectX;

VertexConstantBuffer<TransformCBuffer::Transforms>* TransformCBuffer::mCBuffer = nullptr;
UINT TransformCBuffer::mCount = 0;

TransformCBuffer::TransformCBuffer(Renderer* renderer, Actor* actor, UINT slot)
	:
	mOwner(actor)
{
	mCount++;

	if (!mCBuffer)
	{
		mCBuffer = new VertexConstantBuffer<Transforms>(renderer, slot);
	}
}

TransformCBuffer::~TransformCBuffer()
{
	mCount--;

	if (mCount == 0)
	{
		delete mCBuffer;
		mCBuffer = nullptr;
	}
}

void TransformCBuffer::Bind(Renderer* renderer)
{
	dx::XMMATRIX view = renderer->GetViewMatrix();
	dx::XMMATRIX projection = renderer->GetProjectionMatrix();
	dx::XMMATRIX worldTransform = mOwner->GetWorldTransform();

	const Transforms tf =
	{
		dx::XMMatrixTranspose(worldTransform),
		dx::XMMatrixTranspose(view * projection)
	};

	mCBuffer->Update(renderer, tf);
	mCBuffer->Bind(renderer);
}

void TransformCBuffer::Bind(Renderer* renderer, float width, float height)
{
	dx::XMMATRIX projection = renderer->GetProjectionMatrix2D();
	dx::XMMATRIX worldTransform = mOwner->GetWorldTransform();
	dx::XMMATRIX scaleMat = dx::XMMatrixScaling(width, height, 1.0f);

	const Transforms tf =
	{
		dx::XMMatrixTranspose(scaleMat * worldTransform),
		dx::XMMatrixTranspose(projection)
	};

	mCBuffer->Update(renderer, tf);
	mCBuffer->Bind(renderer);
}

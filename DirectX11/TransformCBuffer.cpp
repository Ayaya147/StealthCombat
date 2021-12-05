#include "TransformCBuffer.h"
#include "ConstantBuffer.h"
#include "Renderer.h"
#include "Actor.h"

namespace dx = DirectX;

VertexConstantBuffer<TransformCBuffer::Transforms>* TransformCBuffer::mBuffer = nullptr;
UINT TransformCBuffer::mCount = 0;

TransformCBuffer::TransformCBuffer(Renderer* renderer, Actor* actor, UINT slot)
	:
	mOwner(actor)
{
	mCount++;

	if (!mBuffer)
	{
		mBuffer = new VertexConstantBuffer<Transforms>(renderer, slot);
	}
}

TransformCBuffer::~TransformCBuffer()
{
	mCount--;

	if (mCount == 0)
	{
		delete mBuffer;
		mBuffer = nullptr;
	}
}

void TransformCBuffer::Bind(Renderer* renderer)
{
	dx::XMMATRIX view = renderer->GetViewMatrix();
	dx::XMMATRIX projection = renderer->GetProjectionMatrix();
	dx::XMMATRIX modelView = mOwner->GetWorldTransform() * view;

	const Transforms tf =
	{
		dx::XMMatrixTranspose(mOwner->GetWorldTransform()),
		dx::XMMatrixTranspose(view * projection)
	};

	mBuffer->Update(renderer, tf);
	mBuffer->Bind(renderer);
}

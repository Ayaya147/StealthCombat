#include "TransformCBuffer.h"
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
	const auto pos = dx::XMVector3Transform(
		dx::XMVectorSet(0.0f, 0.0f, -4.0f, 0.0f),
		dx::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f)
	);

	dx::XMMATRIX camera = dx::XMMatrixLookAtLH(pos, dx::XMVectorZero(), dx::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
	) * dx::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
	dx::XMMATRIX projection = dx::XMMatrixPerspectiveLH(1, 9.0f / 16.0f, 0.5f, 100.0f);
	dx::XMMATRIX modelView = mOwner->GetWorldTransform() * camera;

	const Transforms tf =
	{
		dx::XMMatrixTranspose(modelView),
		dx::XMMatrixTranspose(modelView * projection)
	};

	mBuffer->Update(renderer, tf);
	mBuffer->Bind(renderer);
}

#include "TransformCBuffer.h"
#include "ConstantBuffer.h"
#include "Renderer.h"
#include "Actor.h"

namespace dx = DirectX;

VertexConstantBuffer<TransformCBuffer::Transforms>* TransformCBuffer::mBuffer = nullptr;
UINT TransformCBuffer::mCount = 0;

TransformCBuffer::TransformCBuffer(Renderer* renderer, Actor* actor, bool is3D, UINT slot)
	:
	mOwner(actor),
	mIs3DObject(is3D)
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
	if (mIs3DObject)
	{
		dx::XMMATRIX view = renderer->GetViewMatrix();
		dx::XMMATRIX projection = renderer->GetProjectionMatrix();
		dx::XMMATRIX worldTransform = mOwner->GetWorldTransform();

		const Transforms tf =
		{
			dx::XMMatrixTranspose(worldTransform),
			dx::XMMatrixTranspose(view * projection)
		};

		mBuffer->Update(renderer, tf);
		mBuffer->Bind(renderer);
	}
	else
	{
		dx::XMMATRIX projection = renderer->GetProjectionMatrix2D();
		//dx::XMMATRIX worldTransform = mOwner->GetWorldTransform();
		dx::XMMATRIX worldTransform = dx::XMMatrixIdentity();
		//dx::XMMATRIX scaleMat = dx::XMMatrixScaling(1000.0f, 100.0f, 1.0f);

		const Transforms tf =
		{
			dx::XMMatrixTranspose(worldTransform),
			dx::XMMatrixTranspose(projection)
		};

		mBuffer->Update(renderer, tf);
		mBuffer->Bind(renderer);
	}
}

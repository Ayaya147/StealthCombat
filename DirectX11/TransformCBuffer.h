#pragma once
#include <DirectXMath.h>
#include "Bindable.h"

class Actor;

template<class C>
class VertexConstantBuffer;

class TransformCBuffer : public Bindable
{
public:
	TransformCBuffer(Renderer* renderer, Actor* actor, UINT slot = 0);
	~TransformCBuffer();

	void Bind(Renderer* renderer) override;

protected:
	struct Transforms
	{
		DirectX::XMMATRIX mWorldTransform;
		DirectX::XMMATRIX mViewProj;
	};

	static UINT mCount;
	Actor* mOwner;

private:
	static VertexConstantBuffer<Transforms>* mBuffer;
};

#pragma once
#include <DirectXMath.h>
#include "Bindable.h"

class Actor;

template<class C>
class VertexConstantBuffer;

class TransformCBuffer : public Bindable
{
public:
	TransformCBuffer(Renderer* renderer, class Actor* actor, UINT slot = 0);
	~TransformCBuffer();

	void Bind(Renderer* renderer) override;

private:
	struct  Transforms
	{
		DirectX::XMMATRIX modelView;
		DirectX::XMMATRIX modelViewProj;
	};

	static VertexConstantBuffer<Transforms>* mBuffer;
	static UINT mCount;
	Actor* mOwner;
};

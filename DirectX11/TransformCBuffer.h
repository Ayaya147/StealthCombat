#pragma once
#include <DirectXMath.h>
#include "Bindable.h"
#include "ConstantBuffer.h"

class TransformCBuffer : public Bindable
{
public:
	TransformCBuffer(class Renderer* renderer, class Actor* actor, UINT slot = 0);
	~TransformCBuffer();

	void Bind(class Renderer* renderer) override;

private:
	struct  Transforms
	{
		DirectX::XMMATRIX modelView;
		DirectX::XMMATRIX modelViewProj;
	};

	static VertexConstantBuffer<Transforms>* mBuffer;
	static UINT mCount;
	class Actor* mOwner;
};

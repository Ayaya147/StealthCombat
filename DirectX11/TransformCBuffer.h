#pragma once
#include <DirectXMath.h>
#include "Bindable.h"

template<class C>
class VertexConstantBuffer;

class TransformCBuffer : public Bindable
{
public:
	TransformCBuffer(class Renderer* renderer, class Actor* actor, UINT slot = 0);
	~TransformCBuffer();

	void Bind(class Renderer* renderer) override;
	void Bind(class Renderer* renderer, float width, float height);

private:
	struct Transforms
	{
		DirectX::XMMATRIX mWorldTransform;
		DirectX::XMMATRIX mViewProj;
	};

	static VertexConstantBuffer<Transforms>* mBuffer;
	static UINT mCount;
	class Actor* mOwner;
};

#pragma once
#include "TransformCBuffer.h"

template<class C>
class PixelConstantBuffer;

class TransformCBufferDouble : TransformCBuffer
{
public:
	TransformCBufferDouble(Renderer* renderer, Actor* actor, UINT slot = 0);
	~TransformCBufferDouble();

	void Bind(Renderer* renderer) override;
	void UpdateBindImpl(Renderer* renderer, const Transforms& tf) noexcept;

private:
	static PixelConstantBuffer<Transforms>* mPBuffer;
};

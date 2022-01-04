#include "TransformCBufferDouble.h"
#include "Renderer.h"
#include "Actor.h"

TransformCBufferDouble::TransformCBufferDouble(Renderer* renderer, Actor* actor, UINT slot)
	:
	TransformCBuffer(renderer,actor,slot)
{
}

void TransformCBufferDouble::Bind(Renderer* renderer)
{
}

void TransformCBufferDouble::UpdateBindImpl(Renderer* renderer, const Transforms& tf) noexcept
{
}

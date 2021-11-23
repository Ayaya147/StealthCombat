#include "Topology.h"
#include "Renderer.h"

Topology::Topology(Renderer* renderer, D3D11_PRIMITIVE_TOPOLOGY type)
	:
	mType(type)
{
}

void Topology::Bind(Renderer* renderer)
{
	renderer->GetContext()->IASetPrimitiveTopology(mType);
}

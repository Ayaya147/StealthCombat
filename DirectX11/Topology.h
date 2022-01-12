#pragma once
#include "Bindable.h"

class Topology : public Bindable
{
public:
	Topology(class Renderer* renderer, D3D11_PRIMITIVE_TOPOLOGY type);

	void Bind(class Renderer* renderer) override;

private:
	D3D11_PRIMITIVE_TOPOLOGY mType;
};

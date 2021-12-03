#pragma once
#include "Bindable.h"

class Topology : public Bindable
{
public:
	Topology(Renderer* renderer, D3D11_PRIMITIVE_TOPOLOGY type);

	void Bind(Renderer* renderer) override;

private:
	D3D11_PRIMITIVE_TOPOLOGY mType;
};


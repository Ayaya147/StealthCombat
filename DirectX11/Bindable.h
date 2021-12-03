#pragma once
#include <wrl.h>
#include <d3d11.h>

class Renderer;

class Bindable
{
public:
	Bindable() = default;
	virtual ~Bindable() = default;

	virtual void Bind(Renderer* renderer) = 0;
};

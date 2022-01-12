#pragma once
#include <wrl.h>
#include <d3d11.h>

class Bindable
{
public:
	Bindable() = default;
	virtual ~Bindable() = default;

	virtual void Bind(class Renderer* renderer) = 0;
};

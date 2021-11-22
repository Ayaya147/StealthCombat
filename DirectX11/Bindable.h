#pragma once
#include <wrl.h>
#include <d3d11.h>
#include "DxException.h"

class Bindable
{
public:
	Bindable();
	virtual ~Bindable();

	virtual void Bind(class Renderer* renderer) = 0;
};

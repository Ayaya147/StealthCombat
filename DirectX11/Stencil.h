#pragma once
#include "Bindable.h"

class Stencil : public Bindable
{
public:
	enum class Mode
	{
		EOff,
		EOn,
	};

	Stencil(class Renderer* renderer, Mode mode);
	void Bind(class Renderer* renderer) override;

private:
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> mStencil;
};

#include "Stencil.h"
#include "Renderer.h"
#include "DxException.h"

Stencil::Stencil(Renderer* renderer, Mode mode)
{
	D3D11_DEPTH_STENCIL_DESC dsDesc = CD3D11_DEPTH_STENCIL_DESC{ CD3D11_DEFAULT{} };

	if (mode == Mode::EOff)
	{
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		dsDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
	}
	else if (mode == Mode::EOn)
	{
		dsDesc.DepthEnable = TRUE;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	}

	ThrowIfFailed(renderer->GetDevice()->CreateDepthStencilState(&dsDesc, &mStencil));
}

void Stencil::Bind(Renderer* renderer)
{
	renderer->GetContext()->OMSetDepthStencilState(mStencil.Get(), 0xFF);
}

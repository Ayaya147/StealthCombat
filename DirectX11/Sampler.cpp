#include "Sampler.h"
#include "Renderer.h"
#include "DxException.h"

Sampler::Sampler(Renderer* renderer)
{
	D3D11_SAMPLER_DESC samplerDesc = CD3D11_SAMPLER_DESC{ CD3D11_DEFAULT{} };
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MaxAnisotropy = D3D11_REQ_MAXANISOTROPY;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MinLOD = 0.0f;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	ThrowIfFailed(renderer->GetDevice()->CreateSamplerState(&samplerDesc, &mSampler));
}

void Sampler::Bind(Renderer* renderer)
{
	renderer->GetContext()->PSSetSamplers(0, 1, mSampler.GetAddressOf());
}

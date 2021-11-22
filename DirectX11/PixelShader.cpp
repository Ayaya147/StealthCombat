#include <d3dcompiler.h>
#include "PixelShader.h"
#include "Renderer.h"

PixelShader::PixelShader(Renderer* renderer, const std::wstring& fileName)
{
	Microsoft::WRL::ComPtr<ID3DBlob> blob;
	ThrowIfFailed(D3DReadFileToBlob(fileName.c_str(), &blob));
	ThrowIfFailed(renderer->GetDevice()->CreatePixelShader(
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		nullptr,
		&mPixelShader
	));
}

void PixelShader::Bind(Renderer* renderer)
{
	renderer->GetContext()->PSSetShader(mPixelShader.Get(), nullptr, 0);
}

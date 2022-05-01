#include <d3dcompiler.h>
#include "ComputeShader.h"
#include "Renderer.h"
#include "DxException.h"

ComputeShader::ComputeShader(Renderer* renderer, const std::wstring& fileName)
{
	Microsoft::WRL::ComPtr<ID3DBlob> blob;
	ThrowIfFailed(D3DReadFileToBlob(fileName.c_str(), &blob));
	ThrowIfFailed(renderer->GetDevice()->CreateComputeShader(
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		nullptr,
		&mComputeShader
	));
}

void ComputeShader::Bind(Renderer* renderer)
{
	renderer->GetContext()->CSSetShader(mComputeShader.Get(), nullptr, 0);
}

#include <d3dcompiler.h>
#include "GeometryShader.h"
#include "Renderer.h"
#include "DxException.h"

GeometryShader::GeometryShader(Renderer* renderer, const std::wstring& fileName)
{
	Microsoft::WRL::ComPtr<ID3DBlob> blob;
	ThrowIfFailed(D3DReadFileToBlob(fileName.c_str(), &blob));
	ThrowIfFailed(renderer->GetDevice()->CreateGeometryShader(
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		nullptr,
		&mGeometryShader
	));
}

void GeometryShader::Bind(Renderer* renderer)
{
	renderer->GetContext()->GSSetShader(mGeometryShader.Get(), nullptr, 0);
}

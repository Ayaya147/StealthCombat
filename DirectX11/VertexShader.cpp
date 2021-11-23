#include <d3dcompiler.h>
#include "VertexShader.h"
#include "Renderer.h"
#include "DxException.h"

VertexShader::VertexShader(Renderer* renderer, const std::wstring& fileName)
{
	ThrowIfFailed(D3DReadFileToBlob(fileName.c_str(), &mBytecodeBlod));
	ThrowIfFailed(renderer->GetDevice()->CreateVertexShader(
		mBytecodeBlod->GetBufferPointer(),
		mBytecodeBlod->GetBufferSize(),
		nullptr,
		&mVertexShader
	));
}

void VertexShader::Bind(Renderer* renderer)
{
	renderer->GetContext()->VSSetShader(mVertexShader.Get(), nullptr, 0);
}

ID3DBlob* VertexShader::GetByteCode() const
{
	return mBytecodeBlod.Get();
}

#include "InputLayout.h"
#include "Renderer.h"
#include "VertexShader.h"
#include "DxException.h"

InputLayout::InputLayout(
	Renderer* renderer,
	const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout,
	VertexShader* vs)
{
	ThrowIfFailed(renderer->GetDevice()->CreateInputLayout(
		layout.data(), static_cast<UINT>(layout.size()),
		vs->GetByteCode()->GetBufferPointer(),
		vs->GetByteCode()->GetBufferSize(),
		&mInputLayout
	));
}

void InputLayout::Bind(Renderer* renderer)
{
	renderer->GetContext()->IASetInputLayout(mInputLayout.Get());
}

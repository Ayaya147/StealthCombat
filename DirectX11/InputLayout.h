#pragma once
#include <vector>
#include "Bindable.h"

class InputLayout : public Bindable
{
public:
	InputLayout(
		class Renderer* renderer,
		const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout,
		class VertexShader* vs
	);

	void Bind(class Renderer* renderer) override;

private:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> mInputLayout;
};

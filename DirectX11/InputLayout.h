#pragma once
#include <vector>
#include "Bindable.h"

class VertexShader;

class InputLayout : public Bindable
{
public:
	InputLayout(
		Renderer* renderer,
		const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout,
		VertexShader* vs
	);

	void Bind(Renderer* renderer) override;

private:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> mInputLayout;
};

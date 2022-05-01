#pragma once
#include <string>
#include "Bindable.h"

class ComputeShader : public Bindable
{
public:
	ComputeShader(class Renderer* renderer, const std::wstring& fileName);

	void Bind(class Renderer* renderer) override;

private:
	Microsoft::WRL::ComPtr<ID3D11ComputeShader> mComputeShader;
};

#pragma once
#include <string>
#include "Bindable.h"

class PixelShader : public Bindable
{
public:
	PixelShader(Renderer* renderer, const std::wstring& fileName);

	void Bind(Renderer* renderer) override;

private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> mPixelShader;
};


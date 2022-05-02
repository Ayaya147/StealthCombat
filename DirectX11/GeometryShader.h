#pragma once
#include <string>
#include "Bindable.h"

class GeometryShader : public Bindable
{
public:
	GeometryShader(class Renderer* renderer, const std::wstring& fileName);

	void Bind(class Renderer* renderer) override;

private:
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> mGeometryShader;
};

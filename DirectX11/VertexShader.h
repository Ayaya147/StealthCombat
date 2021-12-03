#pragma once
#include <string>
#include "Bindable.h"

class VertexShader : public Bindable
{
public:
	VertexShader(Renderer* renderer, const std::wstring& fileName);

	void Bind(Renderer* renderer) override;
	ID3DBlob* GetByteCode() const;
	
private:
	Microsoft::WRL::ComPtr<ID3DBlob> mBytecodeBlod;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> mVertexShader;
};

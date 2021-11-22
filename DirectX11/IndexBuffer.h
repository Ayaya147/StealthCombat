#pragma once
#include <vector>
#include "Bindable.h"

class IndexBuffer : public Bindable
{
public:
	IndexBuffer(class Renderer* renderer, const std::vector<unsigned short>& indices);

	void Bind(class Renderer* renderer) override;
	UINT GetCount() const;

private:
	UINT mCount;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mIndexBuffer;
};

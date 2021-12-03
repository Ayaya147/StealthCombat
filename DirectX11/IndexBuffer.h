#pragma once
#include <vector>
#include "Bindable.h"

class IndexBuffer : public Bindable
{
public:
	IndexBuffer(Renderer* renderer, const std::vector<unsigned short>& indices);

	void Bind(Renderer* renderer) override;
	UINT GetCount() const;

private:
	UINT mCount;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mIndexBuffer;
};

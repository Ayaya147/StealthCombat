#pragma once
#include <vector>
#include "Bindable.h"
#include "Renderer.h"
#include "DxException.h"

class VertexBuffer : public Bindable
{
public:
	template<class V>
	VertexBuffer(Renderer* renderer, const std::vector<V>& vertices)
		:
		mStride(sizeof(V))
	{
		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.ByteWidth = static_cast<UINT>(mStride * vertices.size());
		bd.StructureByteStride = mStride;

		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = vertices.data();
		ThrowIfFailed(renderer->GetDevice()->CreateBuffer(&bd, &sd, &mVertexBuffer));
	}

	void Bind(class Renderer* renderer) override;

private:
	UINT mStride;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer;
};

#include "IndexBuffer.h"
#include "Renderer.h"

IndexBuffer::IndexBuffer(Renderer* renderer, const std::vector<unsigned short>& indices)
	:
	mCount(static_cast<UINT>(indices.size()))
{
	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.ByteWidth = static_cast<UINT>(mCount * sizeof(unsigned short));
	bd.StructureByteStride = sizeof(unsigned short);

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = indices.data();
	ThrowIfFailed(renderer->GetDevice()->CreateBuffer(&bd, &sd, &mIndexBuffer));
}

void IndexBuffer::Bind(Renderer* renderer)
{
	renderer->GetContext()->IASetIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
}

UINT IndexBuffer::GetCount() const
{
	return mCount;
}

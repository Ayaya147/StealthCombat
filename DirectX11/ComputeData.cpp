#include "ComputeData.h"
#include "Renderer.h"
#include "DxException.h"

ComputeData::ComputeData(Renderer* renderer, int dataSize, int structSize, void* initData)
{
	InitBuf(renderer, dataSize, structSize, initData);
	InitUAV(renderer);
	InitSRV(renderer);
}

ComputeData::~ComputeData()
{
}

void ComputeData::InitBuf(Renderer* renderer, int dataSize, int structSize, void* initData)
{
	D3D11_BUFFER_DESC desc = {};
	desc.ByteWidth = dataSize;
	desc.StructureByteStride = structSize;
	desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.CPUAccessFlags = 0;

	if (initData)
	{
		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = initData;
		ThrowIfFailed(renderer->GetDevice()->CreateBuffer(&desc, &data, &mBuffer));
	}
	else
	{
		ThrowIfFailed(renderer->GetDevice()->CreateBuffer(&desc, nullptr, &mBuffer));
	}
}

void ComputeData::InitUAV(Renderer* renderer)
{
	D3D11_BUFFER_DESC bufDesc = {};
	mBuffer->GetDesc(&bufDesc);

	D3D11_UNORDERED_ACCESS_VIEW_DESC UAVdesc = {};
	UAVdesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	UAVdesc.Buffer.FirstElement = 0;
	UAVdesc.Format = DXGI_FORMAT_UNKNOWN;
	UAVdesc.Buffer.NumElements = bufDesc.ByteWidth / bufDesc.StructureByteStride;

	ThrowIfFailed(renderer->GetDevice()->CreateUnorderedAccessView(mBuffer.Get(), &UAVdesc, &mUAV));
}

void ComputeData::InitSRV(Renderer* renderer)
{
	D3D11_BUFFER_DESC bufDesc = {};
	mBuffer->GetDesc(&bufDesc);

	D3D11_SHADER_RESOURCE_VIEW_DESC SRVdesc = {};
	SRVdesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	SRVdesc.Format = DXGI_FORMAT_UNKNOWN;
	SRVdesc.BufferEx.FirstElement = 0;
	SRVdesc.BufferEx.NumElements = bufDesc.ByteWidth / bufDesc.StructureByteStride;

	ThrowIfFailed(renderer->GetDevice()->CreateShaderResourceView(mBuffer.Get(), &SRVdesc, &mSRV));
}

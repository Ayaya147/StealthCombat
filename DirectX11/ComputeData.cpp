#include "ComputeData.h"
#include "Renderer.h"

ComputeData::~ComputeData()
{
	buf->Release();
	srv->Release();
	uav->Release();
}

void ComputeData::InitBuf(Renderer* renderer, int data_size, int structSize, void* initData)
{
	D3D11_BUFFER_DESC desc = {};
	desc.ByteWidth = data_size;
	desc.StructureByteStride = structSize;
	desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.CPUAccessFlags = 0;
	if (initData != nullptr)
	{
		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = initData;
		renderer->GetDevice()->CreateBuffer(&desc, &data, &buf);
	}
	else
	{
		renderer->GetDevice()->CreateBuffer(&desc, nullptr, &buf);
	}
}

void ComputeData::InitUAV(Renderer* renderer)
{
	D3D11_BUFFER_DESC bufDesc = {};
	buf->GetDesc(&bufDesc);

	D3D11_UNORDERED_ACCESS_VIEW_DESC UAVdesc = {};
	UAVdesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	UAVdesc.Buffer.FirstElement = 0;
	UAVdesc.Format = DXGI_FORMAT_UNKNOWN;
	UAVdesc.Buffer.NumElements = bufDesc.ByteWidth / bufDesc.StructureByteStride;
	renderer->GetDevice()->CreateUnorderedAccessView(buf, &UAVdesc, &uav);
}

void ComputeData::InitSRV(Renderer* renderer)
{
	D3D11_BUFFER_DESC bufDesc = {};
	buf->GetDesc(&bufDesc);

	D3D11_SHADER_RESOURCE_VIEW_DESC SRVdesc = {};
	SRVdesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	SRVdesc.Format = DXGI_FORMAT_UNKNOWN;
	SRVdesc.BufferEx.FirstElement = 0;
	SRVdesc.BufferEx.NumElements = bufDesc.ByteWidth / bufDesc.StructureByteStride;

	renderer->GetDevice()->CreateShaderResourceView(buf, &SRVdesc, &srv);
}

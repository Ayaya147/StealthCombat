#pragma once
#include "Bindable.h"
#include "Renderer.h"
#include "DxException.h"

template<class C>
class ConstantBuffer : public Bindable
{
public:
	ConstantBuffer(Renderer* renderer, const C& consts, UINT slot = 0)
		:
		mSlot(slot)
	{
		D3D11_BUFFER_DESC bd = {};

		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = 0;
		bd.ByteWidth = sizeof(consts);
		bd.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = &consts;
		ThrowIfFailed(renderer->GetDevice()->CreateBuffer(&bd, &sd, &mConstantBuffer));
	}

	ConstantBuffer(Renderer* renderer, UINT slot = 0)
		:
		mSlot(slot)
	{
		D3D11_BUFFER_DESC bd = {};

		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = 0;
		bd.ByteWidth = sizeof(C);
		bd.StructureByteStride = 0;

		ThrowIfFailed(renderer->GetDevice()->CreateBuffer(&bd, nullptr, &mConstantBuffer));
	}

	void Update(Renderer* renderer, const C& consts)
	{
		D3D11_MAPPED_SUBRESOURCE msr;
		ThrowIfFailed(renderer->GetContext()->Map(mConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr));
		memcpy(msr.pData, &consts, sizeof(consts));
		renderer->GetContext()->Unmap(mConstantBuffer.Get(), 0);
	}

protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> mConstantBuffer;
	UINT mSlot;
};

template<class C>
class VertexConstantBuffer : public ConstantBuffer<C>
{
public:
	using ConstantBuffer<C>::ConstantBuffer;
	using ConstantBuffer<C>::mConstantBuffer;
	using ConstantBuffer<C>::mSlot;

	void Bind(Renderer* renderer) override
	{
		renderer->GetContext()->VSSetConstantBuffers(mSlot, 1, mConstantBuffer.GetAddressOf());
	}
};

template<class C>
class PixelConstantBuffer : public ConstantBuffer<C>
{
public:
	using ConstantBuffer<C>::ConstantBuffer;
	using ConstantBuffer<C>::mConstantBuffer;
	using ConstantBuffer<C>::mSlot;

	void Bind(Renderer* renderer) override
	{
		renderer->GetContext()->PSSetConstantBuffers(mSlot, 1, mConstantBuffer.GetAddressOf());
	}
};

template<class C>
class ComputeConstantBuffer : public ConstantBuffer<C>
{
public:
	using ConstantBuffer<C>::ConstantBuffer;
	using ConstantBuffer<C>::mConstantBuffer;
	using ConstantBuffer<C>::mSlot;

	void Bind(Renderer* renderer) override
	{
		renderer->GetContext()->CSSetConstantBuffers(mSlot, 1, mConstantBuffer.GetAddressOf());
	}
};

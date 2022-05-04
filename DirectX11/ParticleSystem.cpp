#include "ParticleSystem.h"
#include <cassert>
#include <functional>
#include "Renderer.h"
#include "BindableCommon.h"
#include "ComputeData.h"

namespace dx = DirectX;
namespace wrl = Microsoft::WRL;

ParticleSystem::ParticleSystem(Renderer* renderer)
	:
	mIsInit(false),
	mIsBackBuffer(false)
{
	mCurrentParticleCount = mData.newParticles;

	int temp = mCurrentParticleCount / 512 + mCurrentParticleCount % 512;
	temp = temp > 1 ? temp : 1;
	temp = temp < D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION ? temp : D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION;
	mData.numDispatch = temp;
	mDispatchBufferData = { 1 ,1, 1 };
	mDispatchBufferData.x = mData.numDispatch;
	
	//
	D3D11_BUFFER_DESC bd = {};
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	bd.MiscFlags = D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(DispatchBuffer);
	bd.StructureByteStride = 0;

	renderer->GetDevice()->CreateBuffer(&bd, nullptr, &mDispatchBuffer);

	//
	bd = {};
	bd.Usage = D3D11_USAGE_STAGING;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	bd.ByteWidth = sizeof(uint32_t);

	renderer->GetDevice()->CreateBuffer(&bd, nullptr, &mCPUParticleCountReadBuffer);

	//
	D3D11_MAPPED_SUBRESOURCE ms = {};
	renderer->GetContext()->Map(mDispatchBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &ms);
	memcpy(ms.pData, &mDispatchBufferData, sizeof(DispatchBuffer));
	renderer->GetContext()->Unmap(mDispatchBuffer, 0);

	//
	bd = {};
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(ParticleConstant);

	D3D11_SUBRESOURCE_DATA sd;
	sd.pSysMem = &mData;
	renderer->GetDevice()->CreateBuffer(&bd, &sd, &mComputeCBufferParticle);

	//
	D3D11_DRAW_INSTANCED_INDIRECT_ARGS diiArgs{ 0 };
	diiArgs.InstanceCount = 1;
	diiArgs.VertexCountPerInstance = mData.maxParticles;
	sd.pSysMem = &diiArgs;

	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	bd.MiscFlags = D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(D3D11_DRAW_INSTANCED_INDIRECT_ARGS);

	renderer->GetDevice()->CreateBuffer(&bd, &sd, &mInstancedDrawBuffer);

	//
	uint32_t count = mCurrentParticleCount;
	for (int i = 0; i < 2; i++)
	{
		mParticles[i] = new ComputeData;
		mParticleCount[i] = new ComputeData;

		mParticles[i]->InitBuf(renderer, mData.maxParticles * sizeof(Particle), sizeof(Particle));
		mParticles[i]->InitUAV(renderer);
		mParticles[i]->InitSRV(renderer);

		mParticleCount[i]->InitBuf(renderer, sizeof(uint32_t), sizeof(uint32_t), (D3D11_CPU_ACCESS_FLAG)0, &count);
		mParticleCount[i]->InitUAV(renderer);
		mParticleCount[i]->InitSRV(renderer);
	}
}

ParticleSystem::~ParticleSystem()
{
	mDispatchBuffer->Release();
	mCPUParticleCountReadBuffer->Release();
	mInstancedDrawBuffer->Release();
	mComputeCBufferParticle->Release();

	for (int i = 0; i < 2; i++)
	{
		delete mParticles[i];
		delete mParticleCount[i];
	}
}

void ParticleSystem::Init(Renderer* renderer, ComputeShader* particleInitShader)
{
	mIsInit = true;

	particleInitShader->Bind(renderer);
	renderer->GetContext()->CSSetConstantBuffers(2, 1, &mComputeCBufferParticle);
	ID3D11ShaderResourceView* srvs[2] = { mParticles[!mIsBackBuffer]->srv,mParticleCount[!mIsBackBuffer]->srv };
	renderer->GetContext()->CSSetShaderResources(0, 2, srvs);
	ID3D11UnorderedAccessView* uavs[2] = { mParticles[mIsBackBuffer]->uav, mParticleCount[mIsBackBuffer]->uav };
	renderer->GetContext()->CSSetUnorderedAccessViews(0, 2, uavs, nullptr);
	renderer->GetContext()->DispatchIndirect(mDispatchBuffer, 0);

	ID3D11ShaderResourceView* nullSRVs[2] = { nullptr,nullptr };
	ID3D11UnorderedAccessView* nullUAVs[2] = { nullptr,nullptr };
	renderer->GetContext()->CSSetShaderResources(0, 2, nullSRVs);
	renderer->GetContext()->CSSetUnorderedAccessViews(0, 2, nullUAVs, nullptr);

	mIsBackBuffer = !mIsBackBuffer;
}

void ParticleSystem::Update(Renderer* renderer, ComputeShader* particleEmitShader, ComputeShader* particleUpdateShader)
{
	ID3D11ShaderResourceView* nullSRVs[2] = { nullptr,nullptr };
	ID3D11UnorderedAccessView* nullUAVs[2] = { nullptr,nullptr };

	UpdateDispatchBuffer(renderer);
	{
		particleEmitShader->Bind(renderer);
		renderer->GetContext()->CSSetConstantBuffers(2, 1, &mComputeCBufferParticle);
		ID3D11ShaderResourceView* srvs[2] = { mParticles[!mIsBackBuffer]->srv,mParticleCount[!mIsBackBuffer]->srv };
		renderer->GetContext()->CSSetShaderResources(0, 2, srvs);
		ID3D11UnorderedAccessView* uavs[2] = { mParticles[mIsBackBuffer]->uav, mParticleCount[mIsBackBuffer]->uav };
		renderer->GetContext()->CSSetUnorderedAccessViews(0, 2, uavs, nullptr);
		renderer->GetContext()->DispatchIndirect(mDispatchBuffer, 0);
		renderer->GetContext()->CSSetShaderResources(0, 2, nullSRVs);
		renderer->GetContext()->CSSetUnorderedAccessViews(0, 2, nullUAVs, nullptr);
		mIsBackBuffer = !mIsBackBuffer;
	}

	{
		particleUpdateShader->Bind(renderer);
		renderer->GetContext()->CSSetConstantBuffers(2, 1, &mComputeCBufferParticle);
		ID3D11ShaderResourceView* srvs[2] = { mParticles[!mIsBackBuffer]->srv,mParticleCount[!mIsBackBuffer]->srv };
		renderer->GetContext()->CSSetShaderResources(0, 2, srvs);
		ID3D11UnorderedAccessView* uavs[2] = { mParticles[mIsBackBuffer]->uav, mParticleCount[mIsBackBuffer]->uav };
		renderer->GetContext()->CSSetUnorderedAccessViews(0, 2, uavs, nullptr);
		renderer->GetContext()->DispatchIndirect(mDispatchBuffer, 0);
		mIsBackBuffer = !mIsBackBuffer;
	}

	renderer->GetContext()->CSSetShaderResources(0, 2, nullSRVs);
	renderer->GetContext()->CSSetUnorderedAccessViews(0, 2, nullUAVs, nullptr);
}

void ParticleSystem::Draw(Renderer* renderer)
{
}

void ParticleSystem::ForceUpdateBuffer(Renderer* renderer)
{
	D3D11_MAPPED_SUBRESOURCE ms{ 0 };
	renderer->GetContext()->Map(mComputeCBufferParticle, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &ms);
	memcpy(ms.pData, &mData, sizeof(ParticleConstant));
	renderer->GetContext()->Unmap(mComputeCBufferParticle, 0);
}

void ParticleSystem::UpdateDispatchBuffer(Renderer* renderer)
{
	int temp = mCurrentParticleCount / 512 + mCurrentParticleCount % 512;
	temp = temp > 1 ? temp : 1;
	temp = temp < D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION ? temp : D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION;
	mData.numDispatch = temp;

	ForceUpdateBuffer(renderer);
	mDispatchBufferData.x = mData.numDispatch;
	D3D11_MAPPED_SUBRESOURCE ms{ 0 };
	renderer->GetContext()->Map(mDispatchBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &ms);
	memcpy(ms.pData, &mDispatchBufferData, sizeof(DispatchBuffer));
	renderer->GetContext()->Unmap(mDispatchBuffer, 0);
}

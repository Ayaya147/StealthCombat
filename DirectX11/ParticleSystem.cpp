#include "ParticleSystem.h"
#include "Renderer.h"
#include "BindableCommon.h"
#include "ComputeData.h"
#include "ImGui/imgui.h"
#include "DxException.h"

namespace dx = DirectX;
namespace wrl = Microsoft::WRL;

ParticleSystem::ParticleSystem(Renderer* renderer)
	:
	mIsInit(false),
	mIsBackBuffer(false),
	mCurrentParticleCount(0),
	mDispatchBufferData({ 1, 1, 1 })
{
	Reset();

	mComputeCBufferParticle = new ComputeConstantBuffer<ParticleConstant>(renderer, 1);

	// create buffer for dispatch indirect
	D3D11_BUFFER_DESC bd = {};
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	bd.MiscFlags = D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(DispatchBuffer);
	bd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = &mDispatchBufferData;
	ThrowIfFailed(renderer->GetDevice()->CreateBuffer(&bd, &sd, &mDispatchBuffer));

	// create buffer for reading particle count on CPU
	bd = {};
	bd.Usage = D3D11_USAGE_STAGING;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	bd.ByteWidth = sizeof(uint32_t);

	ThrowIfFailed(renderer->GetDevice()->CreateBuffer(&bd, nullptr, &mCPUParticleCountReadBuffer));

	// create buffer for instanced draw
	D3D11_DRAW_INSTANCED_INDIRECT_ARGS diiArgs = {};
	diiArgs.InstanceCount = 1;
	diiArgs.VertexCountPerInstance = mData.mMaxParticles;

	sd = {};
	sd.pSysMem = &diiArgs;

	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	bd.MiscFlags = D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(D3D11_DRAW_INSTANCED_INDIRECT_ARGS);

	ThrowIfFailed(renderer->GetDevice()->CreateBuffer(&bd, &sd, &mInstancedDrawBuffer));

	// create particles buffers
	for (int i = 0; i < 2; i++)
	{
		mParticles[i] = new ComputeData(renderer, mData.mMaxParticles * sizeof(Particle), sizeof(Particle));
		mParticleCount[i] = new ComputeData(renderer, sizeof(uint32_t), sizeof(uint32_t), &mCurrentParticleCount);
	}
}

ParticleSystem::~ParticleSystem()
{
	delete mComputeCBufferParticle;

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
	mComputeCBufferParticle->Bind(renderer);
	ID3D11ShaderResourceView* srvs[2] = { mParticles[!mIsBackBuffer]->GetSRV(), mParticleCount[!mIsBackBuffer]->GetSRV() };
	renderer->GetContext()->CSSetShaderResources(0, 2, srvs);
	ID3D11UnorderedAccessView* uavs[2] = { mParticles[mIsBackBuffer]->GetUAV(), mParticleCount[mIsBackBuffer]->GetUAV() };
	renderer->GetContext()->CSSetUnorderedAccessViews(0, 2, uavs, nullptr);
	renderer->GetContext()->DispatchIndirect(mDispatchBuffer.Get(), 0);
	mIsBackBuffer = !mIsBackBuffer;

	ID3D11ShaderResourceView* nullSRVs[2] = { nullptr,nullptr };
	ID3D11UnorderedAccessView* nullUAVs[2] = { nullptr,nullptr };
	renderer->GetContext()->CSSetShaderResources(0, 2, nullSRVs);
	renderer->GetContext()->CSSetUnorderedAccessViews(0, 2, nullUAVs, nullptr);
}

void ParticleSystem::Update(Renderer* renderer, ComputeShader* particleEmitShader, ComputeShader* particleUpdateShader)
{
	ID3D11ShaderResourceView* nullSRVs[2] = { nullptr,nullptr };
	ID3D11UnorderedAccessView* nullUAVs[2] = { nullptr,nullptr };

	UpdateBuffers(renderer);

	// emit new particles
	particleEmitShader->Bind(renderer);
	mComputeCBufferParticle->Bind(renderer);
	ID3D11ShaderResourceView* srvs[2] = { mParticles[!mIsBackBuffer]->GetSRV(), mParticleCount[!mIsBackBuffer]->GetSRV() };
	renderer->GetContext()->CSSetShaderResources(0, 2, srvs);
	ID3D11UnorderedAccessView* uavs[2] = { mParticles[mIsBackBuffer]->GetUAV(), mParticleCount[mIsBackBuffer]->GetUAV() };
	renderer->GetContext()->CSSetUnorderedAccessViews(0, 2, uavs, nullptr);
	renderer->GetContext()->DispatchIndirect(mDispatchBuffer.Get(), 0);
	mIsBackBuffer = !mIsBackBuffer;

	renderer->GetContext()->CSSetShaderResources(0, 2, nullSRVs);
	renderer->GetContext()->CSSetUnorderedAccessViews(0, 2, nullUAVs, nullptr);

	// simulate all particles
	particleUpdateShader->Bind(renderer);
	mComputeCBufferParticle->Bind(renderer);
	ID3D11ShaderResourceView* srvs_[2] = { mParticles[!mIsBackBuffer]->GetSRV(), mParticleCount[!mIsBackBuffer]->GetSRV() };
	renderer->GetContext()->CSSetShaderResources(0, 2, srvs_);
	ID3D11UnorderedAccessView* uavs_[2] = { mParticles[mIsBackBuffer]->GetUAV(), mParticleCount[mIsBackBuffer]->GetUAV() };
	renderer->GetContext()->CSSetUnorderedAccessViews(0, 2, uavs_, nullptr);
	renderer->GetContext()->DispatchIndirect(mDispatchBuffer.Get(), 0);
	mIsBackBuffer = !mIsBackBuffer;

	renderer->GetContext()->CSSetShaderResources(0, 2, nullSRVs);
	renderer->GetContext()->CSSetUnorderedAccessViews(0, 2, nullUAVs, nullptr);
}

void ParticleSystem::Draw(Renderer* renderer)
{
	D3D11_MAPPED_SUBRESOURCE msDraw = {};
	D3D11_MAPPED_SUBRESOURCE msCount = {};
	D3D11_DRAW_INSTANCED_INDIRECT_ARGS instancedArgs = {};

	renderer->GetContext()->CopyResource(mCPUParticleCountReadBuffer.Get(), mParticleCount[mIsBackBuffer]->GetBuffer());
	ThrowIfFailed(renderer->GetContext()->Map(mInstancedDrawBuffer.Get(), 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &msDraw));
	ThrowIfFailed(renderer->GetContext()->Map(mCPUParticleCountReadBuffer.Get(), 0, D3D11_MAP::D3D11_MAP_READ, 0, &msCount));

	instancedArgs.InstanceCount = 1;
	instancedArgs.VertexCountPerInstance = *(int*)msCount.pData;
	memcpy(msDraw.pData, &instancedArgs, sizeof(D3D11_DRAW_INSTANCED_INDIRECT_ARGS));
	mCurrentParticleCount = instancedArgs.VertexCountPerInstance;

	renderer->GetContext()->Unmap(mCPUParticleCountReadBuffer.Get(), 0);
	renderer->GetContext()->Unmap(mInstancedDrawBuffer.Get(), 0);

	renderer->GetContext()->VSSetShaderResources(0, 1, mParticles[!mIsBackBuffer]->GetSRVAddress());
	renderer->GetContext()->DrawInstancedIndirect(mInstancedDrawBuffer.Get(), 0);

	ID3D11ShaderResourceView* nullSRV =  nullptr;
	renderer->GetContext()->VSSetShaderResources(0, 1, &nullSRV);
}

void ParticleSystem::ImGuiWindow()
{
	if (ImGui::Begin("Particle System"))
	{
		ImGui::Text("Particle Count: %i", mCurrentParticleCount);
		ImGui::SliderInt("Particles Per Second", &mData.mRate, 0, 100000);
		ImGui::SliderFloat("Gravity", &mData.mGravity, 0.1f, 100.0f);
		ImGui::SliderFloat("Mass", &mData.mMass, 0.1f, 10000.0f);
		ImGui::SliderFloat2("Lifetime (Min/Max)", &mData.mLifeTimeMin, 0, 30);
		ImGui::SliderFloat2("Scale (Min/Max)", &mData.mScaleMin, 0, 30);
		ImGui::SliderFloat3("Min Velocity", (float*)&mData.mVelocityMin, -200, 200);
		ImGui::SliderFloat3("Max Velocity", (float*)&mData.mVelocityMax, -200, 200);

		if (ImGui::Button("Reset"))
		{
			Reset();
		}
	}
	ImGui::End();
}

void ParticleSystem::Reset()
{
	mData = {
		{ -50, -100, -50, 0 },
		{ 50, -50, 50, 0 },
		{ -200.0f, -200.0f, -200.0f, 0 },
		{ 200.0f, 200.0f, 200.0f, 0 },
		{0, 0, 0, 1},
		2.0f,
		5.0f,
		2.0f,
		4.0f,
		1024 * 512,
		10.0f,
		2000.0f,
		5000,
		1,
	};
}

void ParticleSystem::UpdateBuffers(Renderer* renderer)
{
	// update constant buffer
	int num = mCurrentParticleCount / 512 + mCurrentParticleCount % 512;
	num = num > 1 ? num : 1;
	num = num < D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION ? num : D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION;
	mData.mNumDispatch = num;

	dx::XMFLOAT3 at = renderer->GetViewAtPosition();
	mData.mPosition = { at.x,at.y, at.z, 1.0f };
	mComputeCBufferParticle->Update(renderer, mData);

	// update buffer for dispatch indirect
	mDispatchBufferData.x = mData.mNumDispatch;
	D3D11_MAPPED_SUBRESOURCE ms = {};
	ThrowIfFailed(renderer->GetContext()->Map(mDispatchBuffer.Get(), 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &ms));
	memcpy(ms.pData, &mDispatchBufferData, sizeof(DispatchBuffer));
	renderer->GetContext()->Unmap(mDispatchBuffer.Get(), 0);
}

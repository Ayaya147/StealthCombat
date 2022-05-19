#include "ParticleSystem.h"
#include "Renderer.h"
#include "BindableCommon.h"
#include "ComputeData.h"
#include "ImGui/imgui.h"

namespace dx = DirectX;
namespace wrl = Microsoft::WRL;

ParticleSystem::ParticleSystem(Renderer* renderer)
	:
	mIsInit(false),
	mIsBackBuffer(false)
{
	Reset();

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
		mParticles[i] = new ComputeData();
		mParticleCount[i] = new ComputeData();

		mParticles[i]->InitBuf(renderer, mData.maxParticles * sizeof(Particle), sizeof(Particle));
		mParticles[i]->InitUAV(renderer);
		mParticles[i]->InitSRV(renderer);

		mParticleCount[i]->InitBuf(renderer, sizeof(uint32_t), sizeof(uint32_t), &count);
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
	renderer->GetContext()->CSSetConstantBuffers(1, 1, &mComputeCBufferParticle);
	ID3D11ShaderResourceView* srvs[2] = { mParticles[!mIsBackBuffer]->srv,mParticleCount[!mIsBackBuffer]->srv };
	renderer->GetContext()->CSSetShaderResources(0, 2, srvs);
	ID3D11UnorderedAccessView* uavs[2] = { mParticles[mIsBackBuffer]->uav, mParticleCount[mIsBackBuffer]->uav };
	renderer->GetContext()->CSSetUnorderedAccessViews(0, 2, uavs, nullptr);
	renderer->GetContext()->DispatchIndirect(mDispatchBuffer, 0);
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

	UpdateDispatchBuffer(renderer);

	//
	particleEmitShader->Bind(renderer);
	renderer->GetContext()->CSSetConstantBuffers(1, 1, &mComputeCBufferParticle);
	ID3D11ShaderResourceView* srvs[2] = { mParticles[!mIsBackBuffer]->srv,mParticleCount[!mIsBackBuffer]->srv };
	renderer->GetContext()->CSSetShaderResources(0, 2, srvs);
	ID3D11UnorderedAccessView* uavs[2] = { mParticles[mIsBackBuffer]->uav, mParticleCount[mIsBackBuffer]->uav };
	renderer->GetContext()->CSSetUnorderedAccessViews(0, 2, uavs, nullptr);
	renderer->GetContext()->DispatchIndirect(mDispatchBuffer, 0);
	mIsBackBuffer = !mIsBackBuffer;

	renderer->GetContext()->CSSetShaderResources(0, 2, nullSRVs);
	renderer->GetContext()->CSSetUnorderedAccessViews(0, 2, nullUAVs, nullptr);

	//
	particleUpdateShader->Bind(renderer);
	renderer->GetContext()->CSSetConstantBuffers(1, 1, &mComputeCBufferParticle);
	ID3D11ShaderResourceView* srvs_[2] = { mParticles[!mIsBackBuffer]->srv,mParticleCount[!mIsBackBuffer]->srv };
	renderer->GetContext()->CSSetShaderResources(0, 2, srvs_);
	ID3D11UnorderedAccessView* uavs_[2] = { mParticles[mIsBackBuffer]->uav, mParticleCount[mIsBackBuffer]->uav };
	renderer->GetContext()->CSSetUnorderedAccessViews(0, 2, uavs_, nullptr);
	renderer->GetContext()->DispatchIndirect(mDispatchBuffer, 0);
	mIsBackBuffer = !mIsBackBuffer;

	renderer->GetContext()->CSSetShaderResources(0, 2, nullSRVs);
	renderer->GetContext()->CSSetUnorderedAccessViews(0, 2, nullUAVs, nullptr);
}

void ParticleSystem::Draw(Renderer* renderer)
{
	D3D11_MAPPED_SUBRESOURCE msDraw{};
	D3D11_MAPPED_SUBRESOURCE msCount{};
	D3D11_DRAW_INSTANCED_INDIRECT_ARGS instancedArgs{};

	renderer->GetContext()->CopyResource(mCPUParticleCountReadBuffer, mParticleCount[mIsBackBuffer]->buf);
	renderer->GetContext()->Map(mInstancedDrawBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &msDraw);
	renderer->GetContext()->Map(mCPUParticleCountReadBuffer, 0, D3D11_MAP::D3D11_MAP_READ, 0, &msCount);

	instancedArgs.InstanceCount = 1;
	instancedArgs.VertexCountPerInstance = *(int*)msCount.pData;
	memcpy(msDraw.pData, &instancedArgs, sizeof(D3D11_DRAW_INSTANCED_INDIRECT_ARGS));
	mCurrentParticleCount = instancedArgs.VertexCountPerInstance;

	renderer->GetContext()->Unmap(mCPUParticleCountReadBuffer, 0);
	renderer->GetContext()->Unmap(mInstancedDrawBuffer, 0);

	renderer->GetContext()->VSSetShaderResources(0, 1, &mParticles[!mIsBackBuffer]->srv);
	renderer->GetContext()->DrawInstancedIndirect(mInstancedDrawBuffer, 0);

	ID3D11ShaderResourceView* nullSRVs[1] = { nullptr };
	renderer->GetContext()->VSSetShaderResources(0, 1, nullSRVs);
}

void ParticleSystem::ImGuiWindow()
{
	if (ImGui::Begin("Particle System"))
	{
		ImGui::Text("Particle Count: %i", mCurrentParticleCount);
		ImGui::SliderInt("Particles Per Second", &mData.rate, 0, 100000);
		ImGui::SliderFloat("Gravity", &mData.gravity, 0.1f, 100.0f);
		ImGui::SliderFloat("Mass", &mData.mass, 0.1f, 10000.0f);
		ImGui::SliderFloat2("Lifetime (Min/Max)", &mData.LifeTimeMin, 0, 30);
		ImGui::SliderFloat2("Scale (Min/Max)", &mData.ScaleMin, 0, 30);
		ImGui::SliderFloat3("Min Velocity", (float*)&mData.VelocityMin, -200, 200);
		ImGui::SliderFloat3("Max Velocity", (float*)&mData.VelocityMax, -200, 200);

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
		2.0f,
		5.0f,
		2.0f,
		4.0f,
		1024 * 1,
		1024 * 512,
		10.0f,
		2000.0f,
		5000,
		1,
		0,
		0,
		{ -50, -100, -50, 0 },
		{ 50, -50, 50, 0 },
		{ -200.0f, -200.0f, -200.0f, 0 },
		{ 200.0f, 200.0f, 200.0f, 0 },
		{0, 0, 0, 1}
	};
}

void ParticleSystem::ForceUpdateBuffer(Renderer* renderer)
{
	D3D11_MAPPED_SUBRESOURCE ms{ 0 };
	renderer->GetContext()->Map(mComputeCBufferParticle, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &ms);
	dx::XMFLOAT3 at = renderer->GetViewAtPosition();
	mData.Position = { at.x,at.y, at.z, 1.0f };
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

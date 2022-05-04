#include "ParticleManager.h"
#include "Renderer.h"
#include "BindableCommon.h"
#include "ParticleSystem.h"

namespace dx = DirectX;
namespace wrl = Microsoft::WRL;

ParticleManager::ParticleManager(Renderer* renderer)
{
	std::wstring fileName = L"ShaderBin\\ParticleCSInit.cso";
	mParticleInitShader = new ComputeShader(renderer, fileName);
	fileName = L"ShaderBin\\ParticleCSEmit.cso";
	mParticleEmitShader = new ComputeShader(renderer, fileName);
	fileName = L"ShaderBin\\ParticleCSUpdate.cso";
	mParticleUpdateShader = new ComputeShader(renderer, fileName);
	fileName = L"ShaderBin\\ParticleVS.cso";
	mParticleVertexShader = new VertexShader(renderer, fileName);
	fileName = L"ShaderBin\\ParticlePS.cso";
	mParticlePixelShader = new PixelShader(renderer, fileName);
	fileName = L"ShaderBin\\ParticleGS.cso";
	mParticleGeometryShader = new GeometryShader(renderer, fileName);

	mComputeCBufferSystem = new ComputeConstantBuffer<SystemConstant>(renderer, 0);
	mComputeCBufferCamera = new ComputeConstantBuffer<CameraConstant>(renderer, 1);
	mVertexCBufferCamera = new VertexConstantBuffer<CameraConstant>(renderer, 0);
}

ParticleManager::~ParticleManager()
{
	for (auto ps : mParticleSystems)
	{
		delete ps;
	}

	delete mParticleInitShader;
	delete mParticleEmitShader;
	delete mParticleUpdateShader;
	delete mParticleVertexShader;
	delete mParticlePixelShader;
	delete mParticleGeometryShader;
	delete mComputeCBufferSystem;
	delete mComputeCBufferCamera;
	delete mVertexCBufferCamera;
}

void ParticleManager::CreateParticleSystem(Renderer* renderer)
{
	ParticleSystem* ps = new ParticleSystem(renderer);
	mParticleSystems.emplace_back(ps);
}

void ParticleManager::Update(Renderer* renderer)
{
	mComputeCBufferSystem->Bind(renderer);
	mComputeCBufferCamera->Bind(renderer);

	for (auto ps : mParticleSystems)
	{
		if (!ps->GetIsInit())
		{
			ps->Init(renderer, mParticleInitShader);
		}

		ps->Update(renderer, mParticleEmitShader, mParticleUpdateShader);
	}

	renderer->GetContext()->CSSetShader(nullptr, nullptr, 0);
	ID3D11Buffer* nullBuffs[3] = { nullptr, nullptr, nullptr };
	renderer->GetContext()->CSSetConstantBuffers(0, 3, nullBuffs);
	ID3D11ShaderResourceView* nullSRVs[2] = { nullptr, nullptr };
	renderer->GetContext()->CSSetShaderResources(0, 2, nullSRVs);
	ID3D11UnorderedAccessView* nullViews[2] = { nullptr,nullptr };
	renderer->GetContext()->CSSetUnorderedAccessViews(0, 2, nullViews, 0);
}

void ParticleManager::Draw(Renderer* renderer)
{
	ID3D11Buffer* nullBuffs[3] = { nullptr, nullptr, nullptr};
	uint32_t nullstrides[1] = { 0 };
	renderer->GetContext()->IASetInputLayout(nullptr);
	renderer->GetContext()->IASetIndexBuffer(nullptr, DXGI_FORMAT::DXGI_FORMAT_UNKNOWN, 0);
	renderer->GetContext()->IASetVertexBuffers(0, 1, nullBuffs, nullstrides, nullstrides);
	mParticleVertexShader->Bind(renderer);
	mParticlePixelShader->Bind(renderer);

	//
	mVertexCBufferCamera->Bind(renderer);
	//

	for (auto ps : mParticleSystems)
	{
		ps->Draw(renderer);
	}

	renderer->GetContext()->CSSetShader(nullptr, nullptr, 0);
	renderer->GetContext()->CSSetConstantBuffers(0, 3, nullBuffs);
	ID3D11ShaderResourceView* nullSRVs[2] = { nullptr, nullptr };
	renderer->GetContext()->CSSetShaderResources(0, 2, nullSRVs);
	ID3D11UnorderedAccessView* nullViews[2] = { nullptr,nullptr };
	renderer->GetContext()->CSSetUnorderedAccessViews(0, 2, nullViews, 0);
}

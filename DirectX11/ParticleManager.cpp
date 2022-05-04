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
}

void ParticleManager::Draw(Renderer* renderer)
{
	for (auto ps : mParticleSystems)
	{
		ps->Draw(renderer);
	}
}

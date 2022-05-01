#include "ParticleSystem.h"
#include "Renderer.h"
#include "ComputeShader.h"

namespace dx = DirectX;
namespace wrl = Microsoft::WRL;

ParticleSystem::ParticleSystem(Renderer* renderer)
	:
	mRenderer(renderer)
{
	std::wstring CSName = L"ShaderBin\\ParticleCSInit.cso";
	mParticleInitShader = new ComputeShader(renderer, CSName);
	CSName = L"ShaderBin\\ParticleCSEmit.cso";
	mParticleEmitShader = new ComputeShader(renderer, CSName);
	CSName = L"ShaderBin\\ParticleCSUpdate.cso";
	mParticleUpdateShader = new ComputeShader(renderer, CSName);
}

ParticleSystem::~ParticleSystem()
{
	delete mParticleInitShader;
	delete mParticleEmitShader;
	delete mParticleUpdateShader;
}

void ParticleSystem::Update(Renderer* renderer)
{
}

void ParticleSystem::Draw(Renderer* renderer)
{
}

#include "ParticleSystem.h"
#include "Renderer.h"
#include "BindableCommon.h"

namespace dx = DirectX;
namespace wrl = Microsoft::WRL;

ParticleSystem::ParticleSystem(Renderer* renderer)
	:
	mIsInit(false)
{
}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::Init(Renderer* renderer)
{
	mIsInit = true;
}

void ParticleSystem::Update(Renderer* renderer)
{
}

void ParticleSystem::Draw(Renderer* renderer)
{
}

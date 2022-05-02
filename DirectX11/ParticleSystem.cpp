#include "ParticleSystem.h"
#include "Renderer.h"
#include "ComputeShader.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "GeometryShader.h"
#include "ConstantBuffer.h"

namespace dx = DirectX;
namespace wrl = Microsoft::WRL;

ParticleSystem::ParticleSystem(Renderer* renderer)
	:
	mRenderer(renderer),
	mIsInit(false)
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

ParticleSystem::~ParticleSystem()
{
	delete mParticleInitShader;
	delete mParticleEmitShader;
	delete mParticleUpdateShader;
	delete mParticleVertexShader;
	delete mParticlePixelShader;
	delete mParticleGeometryShader;
}

void ParticleSystem::Update(Renderer* renderer)
{
}

void ParticleSystem::Draw(Renderer* renderer)
{
}

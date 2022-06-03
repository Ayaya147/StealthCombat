#include "ParticleManager.h"
#include "Renderer.h"
#include "BindableCommon.h"
#include "ParticleSystem.h"
#include "BaseScene.h"
#include "GameScene.h"
#include "Window.h"
#include "PlayerActor.h"

namespace dx = DirectX;
namespace wrl = Microsoft::WRL;

ParticleManager::ParticleManager(BaseScene* scene, Renderer* renderer)
	:
	mScene(scene)
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
	delete mVertexCBufferCamera;
}

void ParticleManager::CreateParticleSystem(Renderer* renderer)
{
	ParticleSystem* ps = new ParticleSystem(renderer);
	mParticleSystems.emplace_back(ps);

	ps->Init(renderer, mParticleInitShader);
}

void ParticleManager::Update(Renderer* renderer)
{
	// update constant buffers
	SystemConstant sc = {};
	sc.mDeltaTime = mScene->GetDeltaTime();
	sc.mRandom = static_cast<float>(rand());

	mComputeCBufferSystem->Update(renderer, sc);
	mComputeCBufferSystem->Bind(renderer);

	CameraConstant cc = {};
	cc.mProjectionMatrix = dx::XMMatrixTranspose(mScene->GetRenderer()->GetProjectionMatrix());
	cc.mViewMatrix = dx::XMMatrixTranspose(mScene->GetRenderer()->GetViewMatrix());

	mVertexCBufferCamera->Update(renderer, cc);

	// update emit particles number
	if (auto game = dynamic_cast<GameScene*>(mScene))
	{
		PlayerActor* player = game->GetPlayer();
		if (player->GetOutCloudTime() > 10.0f || player->GetIsInCloud())
		{
			mParticleSystems[0]->SetEmitParticleNumber(5000);
		}
		else
		{
			mParticleSystems[0]->SetEmitParticleNumber(0);
		}
	}

	// update particle systems
	for (auto ps : mParticleSystems)
	{
		ps->Update(renderer, mParticleEmitShader, mParticleUpdateShader);
	}

	renderer->GetContext()->CSSetShader(nullptr, nullptr, 0);
}

void ParticleManager::Draw(Renderer* renderer)
{
	ID3D11Buffer* nullBuff = nullptr;
	uint32_t nullstride = 0;

	renderer->GetContext()->IASetInputLayout(nullptr);
	renderer->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	renderer->GetContext()->IASetIndexBuffer(nullptr, DXGI_FORMAT::DXGI_FORMAT_UNKNOWN, 0);
	renderer->GetContext()->IASetVertexBuffers(0, 1, &nullBuff, &nullstride, &nullstride);
	mParticleVertexShader->Bind(renderer);
	mParticlePixelShader->Bind(renderer);
	mParticleGeometryShader->Bind(renderer);
	mVertexCBufferCamera->Bind(renderer);

	for (auto ps : mParticleSystems)
	{
		ps->Draw(renderer);
	}

	renderer->GetContext()->GSSetShader(nullptr, nullptr, 0);
}

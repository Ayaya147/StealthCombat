#include "ParticleManager.h"
#include "Renderer.h"
#include "BindableCommon.h"
#include "ParticleSystem.h"
#include "BaseScene.h"
#include "Window.h"
#include "Random.h"

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
	mComputeCBufferCamera = new ComputeConstantBuffer<CameraConstant>(renderer, 1);
	mVertexCBufferCamera = new VertexConstantBuffer<CameraConstant>(renderer, 0);
	mGeometryCBufferSystem = new GeometryConstantBuffer<SystemConstant>(renderer, 0);

	dx::XMFLOAT3 cameraPos = { 0.0f,0.0f,-400.0f };
	dx::XMFLOAT3 at = { 0.0f,0.0f,0.0f };
	dx::XMFLOAT3 up = { 0.0f,1.0f,0.0f };
	mVirtualViewMatrix = dx::XMMatrixLookAtLH(
		dx::XMLoadFloat3(&cameraPos),
		dx::XMLoadFloat3(&at),
		dx::XMLoadFloat3(&up)
	);
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
	delete mGeometryCBufferSystem;
}

void ParticleManager::CreateParticleSystem(Renderer* renderer)
{
	ParticleSystem* ps = new ParticleSystem(renderer);
	mParticleSystems.emplace_back(ps);
}

void ParticleManager::Update(Renderer* renderer)
{
	SystemConstant sc = {};
	sc.mScreenWidth = (float)mScene->GetWindow()->GetClientWidth();
	sc.mScreenHeight = (float)mScene->GetWindow()->GetClientHeight();
	sc.mDeltaTime = mScene->GetDeltaTime();
	sc.mTime = mScene->GetGameTime();
	sc.mFPS = 1.0f / sc.mDeltaTime;
	sc.mD2 = (float)rand();
	sc.mD3 = (float)rand();
	sc.mD1 = (float)rand();

	mComputeCBufferSystem->Update(renderer, sc);
	mComputeCBufferSystem->Bind(renderer);
	mGeometryCBufferSystem->Update(renderer, sc);

	CameraConstant cc = {};
	cc.mProjectionMatrix = mScene->GetRenderer()->GetProjectionMatrix();
	cc.mViewMatrix = mVirtualViewMatrix;
	cc.mInvProjectionMatrix = dx::XMMatrixInverse(nullptr, cc.mProjectionMatrix);
	cc.mInvViewMatrix = dx::XMMatrixInverse(nullptr, cc.mViewMatrix);

	mComputeCBufferCamera->Update(renderer, cc);
	mComputeCBufferCamera->Bind(renderer);
	mVertexCBufferCamera->Update(renderer, cc);

	for (auto ps : mParticleSystems)
	{
		if (!ps->GetIsInit())
		{
			ps->Init(renderer, mParticleInitShader);
		}

		ps->Update(renderer, mParticleEmitShader, mParticleUpdateShader);
	}

	renderer->GetContext()->CSSetShader(nullptr, nullptr, 0);
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
	mParticleGeometryShader->Bind(renderer);
	mVertexCBufferCamera->Bind(renderer);
	mGeometryCBufferSystem->Bind(renderer);

	for (auto ps : mParticleSystems)
	{
		ps->Draw(renderer);
	}

	renderer->GetContext()->GSSetShader(nullptr, nullptr, 0);
}

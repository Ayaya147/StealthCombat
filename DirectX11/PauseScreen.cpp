#include "PauseScreen.h"
#include "Renderer.h"
#include "Bindable.h"
#include "ConstantBuffer.h"
#include "BaseScene.h"
#include "Texture.h"
#include "InputSystem.h"
#include "VertexBuffer.h"

namespace dx = DirectX;

VertexConstantBuffer<PauseScreen::Transforms>* PauseScreen::mCBuffer = nullptr;
int PauseScreen::mCount = 0;

PauseScreen::PauseScreen(BaseScene* scene, Texture* texture)
	:
	mScene(scene),
	mTexture(texture),
	mState(UIState::EClosing),
	mPosition(dx::XMFLOAT3{}),
	mRotation(dx::XMFLOAT3{}),
	mScale(dx::XMFLOAT3{ 1.0f,1.0f,1.0f }),
	mColor(dx::XMFLOAT4{ 1.0f,1.0f,1.0f,1.0f })
{
	mCount++;
	scene->PushUI(this);

	if (!mCBuffer)
	{
		mCBuffer = new VertexConstantBuffer<Transforms>(scene->GetRenderer(), 0);
	}
}

PauseScreen::~PauseScreen()
{
	mCount--;

	if (mCount == 0)
	{
		delete mCBuffer;
		mCBuffer = nullptr;
	}
}

void PauseScreen::ProcessInput()
{
}

void PauseScreen::Update(float deltaTime)
{
}

void PauseScreen::Draw(Renderer* renderer, VertexBuffer* vertexBuffer)
{
	if (mState == UIState::EActive)
	{
		D3D11_MAPPED_SUBRESOURCE msr;
		renderer->GetContext()->Map(vertexBuffer->GetVertexBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
		Renderer::Vertex* vertex = static_cast<Renderer::Vertex*>(msr.pData);

		vertex[0].pos = { -0.5f,-0.5f,0.0f };
		vertex[1].pos = {  0.5f,-0.5f,0.0f };
		vertex[2].pos = { -0.5f, 0.5f,0.0f };
		vertex[3].pos = {  0.5f, 0.5f,0.0f };

		vertex[0].col = mColor;
		vertex[1].col = mColor;
		vertex[2].col = mColor;
		vertex[3].col = mColor;

		vertex[0].tc = { 0.0f,0.0f };
		vertex[1].tc = { 1.0f,0.0f };
		vertex[2].tc = { 0.0f,1.0f };
		vertex[3].tc = { 1.0f,1.0f };

		renderer->GetContext()->Unmap(vertexBuffer->GetVertexBuffer(), 0);
		vertexBuffer->Bind(renderer);

		mCBuffer->Update(renderer, mTransforms);
		mCBuffer->Bind(renderer);
		mTexture->Bind(renderer);

		renderer->GetContext()->Draw(4, 0);
	}
}

void PauseScreen::ComputeWorldTransform()
{
	dx::XMMATRIX worldTransform = dx::XMMatrixScaling(mScale.x, mScale.y, mScale.z);
	worldTransform *= dx::XMMatrixRotationRollPitchYaw(mRotation.x, mRotation.y, mRotation.z);
	worldTransform *= dx::XMMatrixTranslation(mPosition.x, mPosition.y, mPosition.z);
	dx::XMMATRIX scaleMat = dx::XMMatrixScaling(
		static_cast<float>(mTexture->GetTexWidth()),
		static_cast<float>(mTexture->GetTexHeight()),
		1.0f
	);
	dx::XMMATRIX projection = mScene->GetRenderer()->GetProjectionMatrix2D();

	mTransforms = {
		dx::XMMatrixTranspose(scaleMat * worldTransform),
		dx::XMMatrixTranspose(projection)
	};
}

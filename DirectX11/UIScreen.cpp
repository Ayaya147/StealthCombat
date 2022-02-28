#include "UIScreen.h"
#include "Renderer.h"
#include "Bindable.h"
#include "ConstantBuffer.h"
#include "BaseScene.h"
#include "Texture.h"
#include "InputSystem.h"

namespace dx = DirectX;

VertexConstantBuffer<UIScreen::Transforms>* UIScreen::mBuffer = nullptr;
int UIScreen::mCount = 0;

UIScreen::UIScreen(BaseScene* scene, Texture* texture)
	:
	mScene(scene),
	mTexture(texture),
	mState(UIState::EClosing),
	mWorldTransform(dx::XMMATRIX{}),
	mPosition(dx::XMFLOAT3{}),
	mRotation(dx::XMFLOAT3{}),
	mScale(dx::XMFLOAT3{ 1.0f,1.0f,1.0f })
{
	mCount++;
	scene->PushUI(this);

	mWorldTransform = dx::XMMatrixScaling(mScale.x, mScale.y, mScale.z);
	mWorldTransform *= dx::XMMatrixRotationRollPitchYaw(mRotation.x, mRotation.y, mRotation.z);
	mWorldTransform *= dx::XMMatrixTranslation(mPosition.x, mPosition.y, mPosition.z);
	dx::XMMATRIX scaleMat = dx::XMMatrixScaling(
		static_cast<float>(mTexture->GetTexWidth()),
		static_cast<float>(mTexture->GetTexHeight()),
		1.0f
	);
	dx::XMMATRIX projection = scene->GetRenderer()->GetProjectionMatrix2D();

	mTransforms = {
		dx::XMMatrixTranspose(scaleMat * mWorldTransform),
		dx::XMMatrixTranspose(projection)
	};

	if (!mBuffer)
	{
		mBuffer = new VertexConstantBuffer<Transforms>(scene->GetRenderer(), 0);
	}
}

UIScreen::~UIScreen()
{
	mCount--;

	if (mCount == 0)
	{
		delete mBuffer;
		mBuffer = nullptr;
	}
}

void UIScreen::Update(float deltaTime)
{
	if (mState == UIState::EActive)
	{
	}
}

void UIScreen::ProcessInput()
{
	if (mState == UIState::EActive)
	{
		InputSystem* input = mScene->GetInputSystem();

		if (input->GetSceneBack())
		{
			mState = UIState::EClosing;
			mScene->SetSceneState(BaseScene::SceneState::EPlay);
		}
	}
}

void UIScreen::Draw(Renderer* renderer)
{
	if (mState == UIState::EActive)
	{
		mBuffer->Update(renderer, mTransforms);
		mBuffer->Bind(renderer);
		mTexture->Bind(renderer);

		renderer->GetContext()->Draw(4, 0);
	}
}

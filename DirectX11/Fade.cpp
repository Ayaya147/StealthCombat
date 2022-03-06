#include "Fade.h"
#include "BaseScene.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "InputLayout.h"
#include "ConstantBuffer.h"

namespace dx = DirectX;

Fade::Fade(BaseScene* scene)
	:
	mScene(scene),
	mFadeState(FadeState::EFadeIn),
	mRate(1.5f),
	mAlpha(1.0f)
{
	Renderer* renderer = scene->GetRenderer();
	mTexture = renderer->GetTexture("fade");
	mCBuffer = new VertexConstantBuffer<Transforms>(renderer, 0);
}

Fade::~Fade()
{
	delete mCBuffer;
}

void Fade::Update(float deltaTime)
{
	if (mFadeState == FadeState::EFadeIn)
	{
		mAlpha -= mRate * deltaTime;
		if (mAlpha <= 0.0f)
		{
			mAlpha = 0.0f;
			mFadeState = FadeState::EFadeNone;
		}
	}
	else if (mFadeState == FadeState::EFadeOut)
	{
		mAlpha += mRate * deltaTime;
		if (mAlpha >= 1.0f)
		{
			mAlpha = 1.0f;
		}
	}
}

void Fade::Draw(Renderer* renderer, VertexBuffer* vertexBuffer)
{
	if (mFadeState != FadeState::EFadeNone)
	{
		dx::XMMATRIX projection = renderer->GetProjectionMatrix2D();
		dx::XMMATRIX scaleMat = dx::XMMatrixScaling(1920.0f, 1080.0f, 1.0f);

		const Transforms tf =
		{
			dx::XMMatrixTranspose(scaleMat),
			dx::XMMatrixTranspose(projection)
		};

		mCBuffer->Update(renderer, tf);
		mCBuffer->Bind(renderer);
		mTexture->Bind(renderer);

		D3D11_MAPPED_SUBRESOURCE msr;
		renderer->GetContext()->Map(vertexBuffer->GetVertexBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
		Renderer::Vertex* vertex = static_cast<Renderer::Vertex*>(msr.pData);

		vertex[0].pos= { -0.5f,-0.5f,0.0f };
		vertex[1].pos= {  0.5f,-0.5f,0.0f };
		vertex[2].pos= { -0.5f, 0.5f,0.0f };
		vertex[3].pos= {  0.5f, 0.5f,0.0f };

		vertex[0].col = { 1.0f,1.0f,1.0f,mAlpha };
		vertex[1].col = { 1.0f,1.0f,1.0f,mAlpha };
		vertex[2].col = { 1.0f,1.0f,1.0f,mAlpha };
		vertex[3].col = { 1.0f,1.0f,1.0f,mAlpha };

		vertex[0].tc = { 0.0f,0.0f };
		vertex[1].tc = { 1.0f,0.0f };
		vertex[2].tc = { 0.0f,1.0f };
		vertex[3].tc = { 1.0f,1.0f };

		renderer->GetContext()->Unmap(vertexBuffer->GetVertexBuffer(), 0);
		vertexBuffer->Bind(renderer);

		renderer->GetContext()->Draw(4, 0);
	}
}

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
	mRate(2.0f),
	mAlpha(1.0f)
{
	Renderer* renderer = scene->GetRenderer();
	mTexture = renderer->GetTexture("fade");

	std::vector<Vertex> vertices;
	vertices.reserve(4);
	vertices.push_back({ {-0.5f,-0.5f,0.0f},{1.0f,1.0f,1.0f,mAlpha},{0.0f,0.0f} });
	vertices.push_back({ { 0.5f,-0.5f,0.0f},{1.0f,1.0f,1.0f,mAlpha},{1.0f,0.0f} });
	vertices.push_back({ {-0.5f, 0.5f,0.0f},{1.0f,1.0f,1.0f,mAlpha},{0.0f,1.0f} });
	vertices.push_back({ { 0.5f, 0.5f,0.0f},{1.0f,1.0f,1.0f,mAlpha},{1.0f,1.0f} });

	mVertexBuffer = new VertexBuffer(renderer, vertices);
	mBuffer = new VertexConstantBuffer<Transforms>(renderer, 0);
}

Fade::~Fade()
{
	delete mVertexBuffer;
	delete mBuffer;
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

	Renderer* renderer = mScene->GetRenderer();
	D3D11_MAPPED_SUBRESOURCE msr;
	renderer->GetContext()->Map(mVertexBuffer->GetVertexBuffer(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &msr);
	Vertex* vertex = static_cast<Vertex*>(msr.pData);

	for (int i = 0; i < 4; i++)
	{
		vertex[i].col = dx::XMFLOAT4{ 1.0f,1.0f,1.0f,mAlpha };
	}

	renderer->GetContext()->Unmap(mVertexBuffer->GetVertexBuffer(), 0);
}

void Fade::Draw(Renderer* renderer)
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

		mBuffer->Update(renderer, tf);
		mBuffer->Bind(renderer);
		mVertexBuffer->Bind(renderer);
		mTexture->Bind(renderer);

		renderer->GetContext()->Draw(4, 0);
	}
}

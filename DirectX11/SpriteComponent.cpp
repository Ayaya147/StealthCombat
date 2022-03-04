#include "SpriteComponent.h"
#include "Renderer.h"
#include "Actor.h"
#include "BaseScene.h"
#include "Texture.h"
#include "VertexBuffer.h"

namespace dx = DirectX;

SpriteComponent::SpriteComponent(Actor* owner, Texture* tex, int drawOrder)
	:
	Component(owner),
	mDrawOrder(drawOrder),
	mTexture(tex),
	mIsVisible(true),
	mColor(dx::XMFLOAT4{ 1.0f,1.0f,1.0f,1.0f })
{
	GetOwner()->GetScene()->GetRenderer()->AddSprite(this);
}

SpriteComponent::~SpriteComponent()
{
	GetOwner()->GetScene()->GetRenderer()->RemoveSprite(this);
}

void SpriteComponent::Draw(Renderer* renderer, VertexBuffer* vertexBuffer)
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

	GetOwner()->Bind(renderer, static_cast<float>(mTexture->GetTexWidth()), static_cast<float>(mTexture->GetTexHeight()));
	renderer->GetContext()->Draw(4, 0);
}

void SpriteComponent::SetTexture(Texture* tex)
{
	mTexture = tex;
}

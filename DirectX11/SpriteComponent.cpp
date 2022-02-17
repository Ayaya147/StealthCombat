#include "SpriteComponent.h"
#include "Renderer.h"
#include "Actor.h"
#include "BaseScene.h"
#include "Texture.h"

SpriteComponent::SpriteComponent(Actor* owner, int drawOrder)
	:
	Component(owner),
	mDrawOrder(drawOrder),
	mTexture(nullptr),
	mIsVisible(true)
{
	GetOwner()->GetScene()->GetRenderer()->AddSprite(this);
}

SpriteComponent::~SpriteComponent()
{
	GetOwner()->GetScene()->GetRenderer()->RemoveSprite(this);
}

void SpriteComponent::Draw(Renderer* renderer)
{
	GetOwner()->Bind(renderer, static_cast<float>(mTexture->GetTexWidth()), static_cast<float>(mTexture->GetTexHeight()));
	mTexture->Bind(renderer);
	renderer->GetContext()->Draw(4, 0);
}

void SpriteComponent::SetTexture(Texture* tex)
{
	mTexture = tex;
}

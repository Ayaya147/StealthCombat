#include "SpriteComponent.h"
#include "Renderer.h"
#include "Actor.h"
#include "BaseScene.h"
#include "Texture.h"

SpriteComponent::SpriteComponent(Actor* owner, Texture* tex, int drawOrder)
	:
	Component(owner),
	mDrawOrder(drawOrder),
	mTexture(tex),
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
	renderer->GetContext()->Draw(4, 0);
}

void SpriteComponent::SetTexture(Texture* tex)
{
	mTexture = tex;
}

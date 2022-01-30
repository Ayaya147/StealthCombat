#include "SpriteComponent.h"
#include "Renderer.h"
#include "Actor.h"
#include "BaseScene.h"

SpriteComponent::SpriteComponent(Actor* owner, int drawOrder)
	:
	Component(owner),
	mDrawOrder(drawOrder)
{
	GetOwner()->GetScene()->GetRenderer()->AddSprite(this);
}

SpriteComponent::~SpriteComponent()
{
	GetOwner()->GetScene()->GetRenderer()->RemoveSprite(this);
}

void SpriteComponent::Draw(Renderer* renderer)
{
}

void SpriteComponent::SetTexture(Texture* texture)
{
}

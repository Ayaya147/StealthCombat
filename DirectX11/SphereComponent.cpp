#include "SphereComponent.h"
#include "Collision.h"
#include "GameScene.h"
#include "Actor.h"
#include "PhysWorld.h"
#include "XMFloatHelper.h"

SphereComponent::SphereComponent(Actor* owner, int updateOrder)
	:
	Component(owner, updateOrder),
	mSphere(nullptr)
{
	auto game = dynamic_cast<GameScene*>(GetOwner()->GetScene());
	game->GetPhysWorld()->AddSphere(this);
}

SphereComponent::~SphereComponent()
{
	delete mSphere;

	if (auto game = dynamic_cast<GameScene*>(GetOwner()->GetScene()))
	{
		game->GetPhysWorld()->RemoveSphere(this);
	}
}

void SphereComponent::OnUpdateWorldTransform()
{
	mSphere->mCenter = GetOwner()->GetPosition();
}

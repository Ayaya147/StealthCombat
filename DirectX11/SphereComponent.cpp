#include "SphereComponent.h"
#include "Collision.h"
#include "GameScene.h"
#include "Actor.h"
#include "PhysWorld.h"

SphereComponent::SphereComponent(Actor* owner, int updateOrder)
	:
	Component(owner, updateOrder),
	mSphereLast(nullptr),
	mSphereCurrent(nullptr)
{
	auto game = dynamic_cast<GameScene*>(GetOwner()->GetScene());
	game->GetPhysWorld()->AddSphere(this);
}

SphereComponent::~SphereComponent()
{
	delete mSphereLast;
	delete mSphereCurrent;

	if (auto game = dynamic_cast<GameScene*>(GetOwner()->GetScene()))
	{
		game->GetPhysWorld()->RemoveSphere(this);
	}
}

void SphereComponent::OnUpdateWorldTransform()
{
	if (mSphereLast)
	{
		mSphereLast->mCenter = mSphereCurrent->mCenter;
	}
	mSphereCurrent->mCenter = GetOwner()->GetPosition();
}

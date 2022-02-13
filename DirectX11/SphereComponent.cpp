#include "SphereComponent.h"
#include "Collision.h"
#include "GameScene.h"
#include "Actor.h"
#include "PhysWorld.h"
#include "XMFloatHelper.h"

SphereComponent::SphereComponent(Actor* owner, int updateOrder)
	:
	Component(owner, updateOrder),
	mSphere0(nullptr),
	mSphere1(nullptr)
{
	auto game = dynamic_cast<GameScene*>(GetOwner()->GetScene());
	game->GetPhysWorld()->AddSphere(this);
}

SphereComponent::~SphereComponent()
{
	delete mSphere0;
	delete mSphere1;

	if (auto game = dynamic_cast<GameScene*>(GetOwner()->GetScene()))
	{
		game->GetPhysWorld()->RemoveSphere(this);
	}
}

void SphereComponent::OnUpdateWorldTransform()
{
	if (mSphere0)
	{
		mSphere0->mCenter = mSphere1->mCenter;
	}
	mSphere1->mCenter = GetOwner()->GetPosition();
}

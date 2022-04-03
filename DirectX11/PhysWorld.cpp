#include "PhysWorld.h"
#include "Collision.h"
#include "SphereComponent.h"
#include "CloudActor.h"
#include "EnemyActor.h"
#include "PlayerActor.h"
#include "MissileActor.h"
#include "XMFloatHelper.h"

PhysWorld::PhysWorld(BaseScene* scene)
	:
	mScene(scene)
{
}

PhysWorld::~PhysWorld()
{
}

bool PhysWorld::IsCollidedWithCloud(SphereComponent* sc)
{
	for (auto s : mSphereComponents)
	{
		if (auto cloud = dynamic_cast<CloudActor*>(s->GetOwner()))
		{
			if (cloud != sc->GetOwner() && Collision::Intersect(sc->GetSphere(), s->GetSphere()))
			{
				return true;
			}
		}
	}

	return false;
}

bool PhysWorld::IsCollidedWithEnemy(SphereComponent* sc, CollisionInfo& info)
{
	for (auto s : mSphereComponents)
	{
		if (auto enemy = dynamic_cast<EnemyActor*>(s->GetOwner()))
		{
			if (s == enemy->GetBodySphereComp() &&
				(Collision::Intersect(sc->GetSphere(), s->GetSphere()) ||
				Collision::SweptSphere(sc->GetSphereLast(), sc->GetSphere(), s->GetSphereLast(), s->GetSphere())))
			{
				info.mActor = enemy;
				return true;
			}
		}
	}

	return false;
}

bool PhysWorld::IsCollidedWithEnemy(SphereComponent* sc, SphereComponent* sc1)
{
	if (Collision::Intersect(sc->GetSphere(), sc1->GetSphere()) ||
		Collision::SweptSphere(sc->GetSphereLast(), sc->GetSphere(), sc1->GetSphereLast(), sc1->GetSphere()))
	{
		return true;
	}

	return false;
}

bool PhysWorld::IsCollidedWithPlayer(SphereComponent* sc)
{
	for (auto s : mSphereComponents)
	{
		if (auto player = dynamic_cast<PlayerActor*>(s->GetOwner()))
		{
			if (s == player->GetBodySphereComp() &&
				(Collision::Intersect(sc->GetSphere(), s->GetSphere()) ||
				Collision::SweptSphere(sc->GetSphereLast(), sc->GetSphere(), s->GetSphereLast(), s->GetSphere())))
			{
				return true;
			}
		}
	}

	return false;
}

void PhysWorld::AddSphere(SphereComponent* sc)
{
	mSphereComponents.emplace_back(sc);
}

void PhysWorld::RemoveSphere(SphereComponent* sc)
{
	auto iter = std::find(mSphereComponents.begin(), mSphereComponents.end(), sc);
	if (iter != mSphereComponents.end())
	{
		std::iter_swap(iter, mSphereComponents.end() - 1);
		mSphereComponents.pop_back();
	}
}

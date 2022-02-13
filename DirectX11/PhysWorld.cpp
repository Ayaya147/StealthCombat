#include "PhysWorld.h"
#include "Collision.h"
#include "SphereComponent.h"
#include "CloudActor.h"
#include "EnemyActor.h"
#include "MissileActor.h"

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
	for (auto s : mSpheres)
	{
		if (auto cloud = dynamic_cast<CloudActor*>(s->GetOwner()))
		{
			if (Collision::Intersect(sc->GetSphere(), s->GetSphere()))
			{
				return true;
			}
		}
	}

	return false;
}

bool PhysWorld::IsCollidedWithEnemy(SphereComponent* sc, CollisionInfo& info)
{
	for (auto s : mSpheres)
	{
		if (auto enemy = dynamic_cast<EnemyActor*>(s->GetOwner()))
		{
			if (Collision::Intersect(sc->GetSphere(), s->GetSphere()))
			{
				info.mActor = enemy;
				return true;
			}
			else if (Collision::SweptSphere(sc, s))
			{
				return true;
			}
		}
	}

	return false;
}

bool PhysWorld::IsCollidedWithMissile(SphereComponent* sc, CollisionInfo& info)
{
	for (auto s : mSpheres)
	{
		if (auto missile = dynamic_cast<MissileActor*>(s->GetOwner()))
		{
			if (Collision::Intersect(sc->GetSphere(), s->GetSphere()))
			{
				info.mActor = missile;
				return true;
			}
			else if (Collision::SweptSphere(sc, s))
			{
				return true;
			}
		}
	}

	return false;
}

void PhysWorld::AddSphere(SphereComponent* sc)
{
	mSpheres.emplace_back(sc);
}

void PhysWorld::RemoveSphere(SphereComponent* sc)
{
	auto iter = std::find(mSpheres.begin(), mSpheres.end(), sc);
	if (iter != mSpheres.end())
	{
		std::iter_swap(iter, mSpheres.end() - 1);
		mSpheres.pop_back();
	}
}

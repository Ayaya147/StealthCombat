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
			if (Collision::Intersect(sc->GetSphere(), s->GetSphere()) ||
				Collision::SweptSphere(sc->GetSphereLast(), sc->GetSphere(), s->GetSphereLast(), s->GetSphere()))
			{
				info.mActor = enemy;
				return true;
			}
		}
	}

	return false;
}

bool PhysWorld::IsCollidedWithEnemy(SphereComponent * sc, SphereComponent * sc1)
{
	if (Collision::Intersect(sc->GetSphere(), sc1->GetSphere()) ||
		Collision::SweptSphere(sc->GetSphereLast(), sc->GetSphere(), sc1->GetSphereLast(), sc1->GetSphere()))
	{
		return true;
	}

	return false;
}

bool PhysWorld::IsAttackRangeCollidedWithEnemy(SphereComponent* sc, CollisionInfo& info)
{
	for (auto s : mSpheres)
	{
		if (auto enemy = dynamic_cast<EnemyActor*>(s->GetOwner()))
		{
			auto player = dynamic_cast<PlayerActor*>(sc->GetOwner());
			if (Collision::Intersect(sc->GetSphere(), s->GetSphere()) &&
				DXMath::Dot(enemy->GetPosition() - player->GetPosition(), player->GetForward()) > 0.0f)
			{
				info.mActor = enemy;
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
			if (Collision::Intersect(sc->GetSphere(), s->GetSphere()) ||
				Collision::SweptSphere(sc->GetSphereLast(), sc->GetSphere(), s->GetSphereLast(), s->GetSphere()))
			{
				info.mActor = missile;
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

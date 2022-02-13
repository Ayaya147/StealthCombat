#include "PhysWorld.h"
#include "Collision.h"
#include "SphereComponent.h"
#include "CloudActor.h"

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
		if (auto a = dynamic_cast<CloudActor*>(s->GetOwner()))
		{
			if (Intersect(sc->GetSphere(), s->GetSphere()))
			{
				return true;
			}
		}
	}

	return false;
}

void PhysWorld::AddSphere(SphereComponent* sphere)
{
	mSpheres.emplace_back(sphere);
}

void PhysWorld::RemoveSphere(SphereComponent* sphere)
{
	auto iter = std::find(mSpheres.begin(), mSpheres.end(), sphere);
	if (iter != mSpheres.end())
	{
		std::iter_swap(iter, mSpheres.end() - 1);
		mSpheres.pop_back();
	}
}

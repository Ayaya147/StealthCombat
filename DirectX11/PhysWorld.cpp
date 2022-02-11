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

bool PhysWorld::IsCollided(SphereComponent* sphere)
{
	for (auto s : mSpheres)
	{
		if (Intersect(*(sphere->GetSphere()), *(s->GetSphere())))
		{
			if (auto a = dynamic_cast<CloudActor*>(s->GetOwner()))
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

#include "PhysWorld.h"

PhysWorld::PhysWorld(BaseScene* scene)
	:
	mScene(scene)
{
}

PhysWorld::~PhysWorld()
{
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

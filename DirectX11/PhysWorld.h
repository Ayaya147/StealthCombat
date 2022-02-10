#pragma once
#include <vector>

class PhysWorld
{
public:
	PhysWorld(class BaseScene* scene);
	~PhysWorld();

	void AddSphere(class SphereComponent* sphere);
	void RemoveSphere(class SphereComponent* sphere);

private:
	class BaseScene* mScene;
	std::vector<class SphereComponent*> mSpheres;
};

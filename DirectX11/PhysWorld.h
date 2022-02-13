#pragma once
#include <vector>

class PhysWorld
{
public:
	PhysWorld(class BaseScene* scene);
	~PhysWorld();

	bool IsCollidedWithCloud(class SphereComponent* sphere);

	void AddSphere(class SphereComponent* sphere);
	void RemoveSphere(class SphereComponent* sphere);

private:
	class BaseScene* mScene;
	std::vector<class SphereComponent*> mSpheres;
};

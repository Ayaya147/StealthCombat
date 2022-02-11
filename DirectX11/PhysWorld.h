#pragma once
#include <vector>

class PhysWorld
{
public:
	struct CollisionInfo
	{
		class SphereComponent* mSphere;
		class Actor* mActor;
	};

	PhysWorld(class BaseScene* scene);
	~PhysWorld();

	bool IsCollided(class SphereComponent* sphere);

	void AddSphere(class SphereComponent* sphere);
	void RemoveSphere(class SphereComponent* sphere);

private:
	class BaseScene* mScene;
	std::vector<class SphereComponent*> mSpheres;
};

#pragma once
#include <vector>

class PhysWorld
{
public:
	struct CollisionInfo
	{
		class Actor* mActor = nullptr;
	};

	PhysWorld(class BaseScene* scene);
	~PhysWorld();

	bool IsCollidedWithCloud(class SphereComponent* sc);
	bool IsCollidedWithEnemy(class SphereComponent* sc, CollisionInfo& info);
	bool IsCollidedWithEnemy(class SphereComponent* sc, class SphereComponent* sc1);
	bool IsCollidedWithPlayer(class SphereComponent* sc);

	void AddSphere(class SphereComponent* sc);
	void RemoveSphere(class SphereComponent* sc);

private:
	class BaseScene* mScene;
	std::vector<class SphereComponent*> mSphereComponents;
};

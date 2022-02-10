#pragma once
#include "Component.h"

class SphereComponent : public Component
{
public:
	SphereComponent(class Actor* owner, int updateOrder = 100);
	~SphereComponent();

	void OnUpdateWorldTransform() override;

	void SetSphere(struct Sphere* model) { mSphere = model; }
	struct Sphere* GetSphere() const { return mSphere; }

private:
	struct Sphere* mSphere;
};

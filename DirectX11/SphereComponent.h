#pragma once
#include "Component.h"

class SphereComponent : public Component
{
public:
	SphereComponent(class Actor* owner, int updateOrder = 100);
	~SphereComponent();

	void OnUpdateWorldTransform() override;

	void SetSphere(struct Sphere* model) { mSphere1 = model; }
	struct Sphere* GetSphere() const { return mSphere1; }
	void SetSphereLast(struct Sphere* model) { mSphere0 = model; }
	struct Sphere* GetSphereLast() const { return mSphere0; }

private:
	struct Sphere* mSphere0;
	struct Sphere* mSphere1;
};

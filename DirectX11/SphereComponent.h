#pragma once
#include "Component.h"

class SphereComponent : public Component
{
public:
	SphereComponent(class Actor* owner, int updateOrder = 100);
	~SphereComponent();

	void OnUpdateWorldTransform() override;

	void SetSphere(struct Sphere* model) { mSphereCurrent = model; }
	struct Sphere* GetSphere() const { return mSphereCurrent; }
	void SetSphereLast(struct Sphere* model) { mSphereLast = model; }
	struct Sphere* GetSphereLast() const { return mSphereLast; }

private:
	struct Sphere* mSphereLast;
	struct Sphere* mSphereCurrent;
};

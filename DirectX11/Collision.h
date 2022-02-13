#pragma once
#include <DirectXMath.h>

struct Sphere
{
	Sphere(const DirectX::XMFLOAT3& center, float radius);
	bool Contains(const DirectX::XMFLOAT3& point) const;

	DirectX::XMFLOAT3 mCenter;
	float mRadius;
};

class Collision
{
public:
	static bool Intersect(Sphere* a, Sphere* b);
	static bool SweptSphere(class SphereComponent* sc1, class ::SphereComponent* sc2);
};

#pragma once
#include <DirectXMath.h>

struct Sphere
{
	Sphere(const DirectX::XMFLOAT3& center, float radius);
	bool Contains(const DirectX::XMFLOAT3& point) const;

	DirectX::XMFLOAT3 mCenter;
	float mRadius;
};

namespace Collision
{
	bool Intersect(const Sphere* a, const Sphere* b);
	bool SweptSphere(const Sphere* a0, const Sphere* a1, const Sphere* b0, const Sphere* b1);
}

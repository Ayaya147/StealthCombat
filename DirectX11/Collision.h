#pragma once
#include <DirectXMath.h>

struct Sphere
{
	Sphere(const DirectX::XMFLOAT3& center, float radius);
	bool Contains(const DirectX::XMFLOAT3& point) const;

	DirectX::XMFLOAT3 mCenter;
	float mRadius;
};

bool Intersect(const Sphere& a, const Sphere& b);
bool SweptSphere(const Sphere& P0, const Sphere& P1, const Sphere& Q0, const Sphere& Q1);

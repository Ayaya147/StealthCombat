#pragma once
#include <DirectXMath.h>

struct Sphere
{
	Sphere(const DirectX::XMFLOAT3& center, float radius);

	DirectX::XMFLOAT3 mCenter;
	float mRadius;
};

class Collision
{
public:
	static bool Contains(Sphere* sphere, const DirectX::XMFLOAT3& point);
	static bool Intersect(Sphere* a, Sphere* b);
	// if collided between frames
	static bool SweptSphere(Sphere* a0, Sphere* a1, Sphere* b0, Sphere* b1);
};

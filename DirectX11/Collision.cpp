#include "Collision.h"
#include "XMFloatHelper.h"

Sphere::Sphere(const DirectX::XMFLOAT3& center, float radius)
	:
	mCenter(center),
	mRadius(radius)
{
}

bool Sphere::Contains(const DirectX::XMFLOAT3& point) const
{
	float distSq = DXMath::LengthSq(mCenter - point);
	return distSq <= (mRadius * mRadius);
}

bool Intersect(const Sphere& a, const Sphere& b)
{
	float distSq = DXMath::LengthSq(a.mCenter - b.mCenter);
	float sumRadii = a.mRadius + b.mRadius;
	return distSq <= (sumRadii * sumRadii);
}

bool SweptSphere(const Sphere& P0, const Sphere& P1, const Sphere& Q0, const Sphere& Q1)
{
	return false;
}

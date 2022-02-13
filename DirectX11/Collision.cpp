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

bool Collision::Intersect(const Sphere* a, const Sphere* b)
{
	float distSq = DXMath::LengthSq(a->mCenter - b->mCenter);
	float sumRadii = a->mRadius + b->mRadius;
	return distSq <= (sumRadii * sumRadii);
}

bool Collision::SweptSphere(const Sphere* a0, const Sphere* a1, const Sphere* b0, const Sphere* b1)
{
	return false;
}

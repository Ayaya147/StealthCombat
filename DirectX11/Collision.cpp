#include "Collision.h"
#include "XMFloatHelper.h"
#include "SphereComponent.h"

namespace dx = DirectX;

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

bool Intersect(Sphere* a, Sphere* b)
{
	float distSq = DXMath::LengthSq(a->mCenter - b->mCenter);
	float sumRadii = a->mRadius + b->mRadius;
	return distSq <= (sumRadii * sumRadii);
}

bool SweptSphere(SphereComponent* sc1, SphereComponent* sc2)
{
	Sphere* a0 = sc1->GetSphereLast();
	Sphere* a1 = sc1->GetSphere();
	Sphere* b0 = sc2->GetSphereLast();
	Sphere* b1 = sc2->GetSphere();

	dx::XMFLOAT3 X = a0->mCenter - b0->mCenter;
	dx::XMFLOAT3 Y = (a1->mCenter - a0->mCenter) - (b1->mCenter - b0->mCenter);
	float a = DXMath::Dot(Y, Y);
	float b = 2.0f * DXMath::Dot(X, Y);
	float sumRadii = a0->mRadius + b0->mRadius;
	float c = DXMath::Dot(X, X) - sumRadii * sumRadii;
	float disc = b * b - 4.0f * a * c;
	if (disc < 0.0f)
	{
		return false;
	}
	else
	{
		disc = sqrtf(disc);
		float outT = (-b - disc) / (2.0f * a);
		if (outT >= 0.0f && outT <= 1.0f)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

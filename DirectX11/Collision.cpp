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

bool Collision::Contains(Sphere* sphere, const DirectX::XMFLOAT3& point)
{
	float distSq = DXMath::LengthSq(sphere->mCenter - point);
	return distSq <= (sphere->mRadius * sphere->mRadius);
}

bool Collision::Intersect(Sphere* a, Sphere* b)
{
	float distSq = DXMath::LengthSq(a->mCenter - b->mCenter);
	float sumRadii = a->mRadius + b->mRadius;
	return distSq <= (sumRadii * sumRadii);
}

bool Collision::SweptSphere(Sphere* a0, Sphere* a1, Sphere* b0, Sphere* b1)
{
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
		float t = (-b - disc) / (2.0f * a);
		if (t >= 0.0f && t <= 1.0f)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

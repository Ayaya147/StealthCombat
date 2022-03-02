#include "Random.h"
#include "XMFloatHelper.h"

std::mt19937 Random::mGenerator;

void Random::Initialize()
{
	std::random_device rd;
	mGenerator.seed(rd());
}

float Random::GetFloat()
{
	return GetFloatRange(0.0f, 1.0f);
}

float Random::GetFloatRange(float min, float max)
{
	std::uniform_real_distribution<float> dist(min, max);
	return dist(mGenerator);
}

int Random::GetIntRange(int min, int max)
{
	std::uniform_int_distribution<int> dist(min, max);
	return dist(mGenerator);
}

DirectX::XMFLOAT3 Random::GetVector()
{
	float x = GetFloatRange(-1.0f, 1.0f);
	float y = GetFloatRange(-1.0f, 1.0f);
	float z = GetFloatRange(-1.0f, 1.0f);
	DirectX::XMFLOAT3 vector = { x,y,z };

	return DXMath::Normalize(vector);
}

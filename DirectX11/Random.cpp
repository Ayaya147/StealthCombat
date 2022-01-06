#include "Random.h"

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

DirectX::XMFLOAT2 Random::GetVector(const DirectX::XMFLOAT2& min, const DirectX::XMFLOAT2& max)
{
	return {};
	//return min + (max - min) * r;
}

#pragma  once
#include <random>
#include <DirectXMath.h>

class Random
{
public:
	static void Initialize();
	static float GetFloat();
	static float GetFloatRange(float min, float max);
	static int GetIntRange(int min, int max);
	static DirectX::XMFLOAT2 GetVector(const DirectX::XMFLOAT2& min, const DirectX::XMFLOAT2& max);

private:
	static std::mt19937 mGenerator;
};

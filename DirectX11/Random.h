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
	static DirectX::XMFLOAT3 GetVector();

private:
	static std::mt19937 mGenerator;
};

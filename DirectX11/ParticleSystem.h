#pragma once
#include <DirectXMath.h>
#include <d3d11.h>

template<class C>
class ComputeConstantBuffer;

class ParticleSystem
{
public:
	ParticleSystem(class Renderer* renderer);
	~ParticleSystem();

	void Init(class Renderer* renderer, class ComputeShader* particleInitShader);
	void Update(class Renderer* renderer);
	void Draw(class Renderer* renderer);

	bool GetIsInit() { return mIsInit; }

private:
	void ForceUpdateBuffer(class Renderer* renderer);
	void UpdateDispatchBuffer(class Renderer* renderer);

	struct DispatchBuffer
	{
		uint32_t x;
		uint32_t y;
		uint32_t z;
	};

	struct Particle
	{
		float age, scale, dummy1, dummy2;
		DirectX::XMFLOAT4 velocity;
		DirectX::XMFLOAT4 color;
		DirectX::XMFLOAT4 position;
	};

	struct ParticleConstant
	{
		float LifeTimeMin = 10.0f;
		float LifeTimeMax = 20.0f;
		float ScaleMin = 1.0f;
		float ScaleMax = 2.0f;
		uint32_t newParticles = 1024 * 256;
		uint32_t maxParticles = 1024 * 1024 * 2;
		float gravity = 100.0f;
		float mass = 5000.0f;
		int rate = 50000;
		int numDispatch = 1;
		int d2;
		int d3;
		DirectX::XMFLOAT4 VelocityMin = { -30, -30, -30, 0 };
		DirectX::XMFLOAT4 VelocityMax = { 30, 30, 30, 0 };
		DirectX::XMFLOAT4 PositionMin = { -100, -100, -100, 0 };
		DirectX::XMFLOAT4 PositionMax = { 100, 100, 100, 0 };
		DirectX::XMFLOAT4 Position = {0, 0, 0, 1};
	};

	bool mIsInit;
	bool mIsBackBuffer;
	unsigned int mCurrentParticleCount;

	DispatchBuffer mDispatchBufferData;
	ID3D11Buffer* mDispatchBuffer = nullptr;
	ID3D11Buffer* mCPUParticleCountReadBuffer = nullptr;
	ID3D11Buffer* mInstancedDrawBuffer = nullptr;

	ParticleConstant mData;
	ID3D11Buffer* mComputeCBufferParticle;

	class ComputeData* mParticleCount[2];
	class ComputeData* mParticles[2];
};

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
	void Update(class Renderer* renderer, class ComputeShader* particleEmitShader, class ComputeShader* particleUpdateShader);
	void Draw(class Renderer* renderer);
	void ImGuiWindow();

	bool GetIsInit() { return mIsInit; }

private:
	void Reset();
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
		float LifeTimeMin;
		float LifeTimeMax;
		float ScaleMin;
		float ScaleMax;
		uint32_t newParticles;
		uint32_t maxParticles;
		float gravity;
		float mass;
		int rate;
		int numDispatch;
		int d2;
		int d3;
		DirectX::XMFLOAT4 VelocityMin;
		DirectX::XMFLOAT4 VelocityMax;
		DirectX::XMFLOAT4 PositionMin;
		DirectX::XMFLOAT4 PositionMax;
		DirectX::XMFLOAT4 Position;
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

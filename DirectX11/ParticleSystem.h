#pragma once
#include <DirectXMath.h>
#include <d3d11.h>
#include <wrl.h>

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

private:
	void Reset();
	void UpdateBuffers(class Renderer* renderer);

	struct DispatchBuffer
	{
		uint32_t x;
		uint32_t y;
		uint32_t z;
	};

	struct Particle
	{
		DirectX::XMFLOAT4 mVelocity;
		DirectX::XMFLOAT4 mColor;
		DirectX::XMFLOAT4 mPosition;
		float mAge;
		float mScale;
	};

	struct ParticleConstant
	{
		DirectX::XMFLOAT4 mVelocityMin;
		DirectX::XMFLOAT4 mVelocityMax;
		DirectX::XMFLOAT4 mPositionMin;
		DirectX::XMFLOAT4 mPositionMax;
		DirectX::XMFLOAT4 mPosition;
		float mLifeTimeMin;
		float mLifeTimeMax;
		float mScaleMin;
		float mScaleMax;
		uint32_t mMaxParticles;
		float mGravity;
		float mMass;
		int mRate;
		int mNumDispatch;
		float padding[3];
	};

	bool mIsBackBuffer;
	unsigned int mCurrentParticleCount;

	DispatchBuffer mDispatchBufferData;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mDispatchBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mCPUParticleCountReadBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mInstancedDrawBuffer;

	ParticleConstant mData;
	ComputeConstantBuffer<ParticleConstant>* mComputeCBufferParticle;

	class ComputeData* mParticleCount[2];
	class ComputeData* mParticles[2];
};

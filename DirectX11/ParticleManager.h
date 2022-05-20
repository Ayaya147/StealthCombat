#pragma once
#include <DirectXMath.h>
#include <d3d11.h>
#include <vector>

template<class C>
class ComputeConstantBuffer;
template<class C>
class VertexConstantBuffer;

class ParticleManager
{
public:
	ParticleManager(class BaseScene* scene, class Renderer* renderer);
	~ParticleManager();

	void CreateParticleSystem(class Renderer* renderer);
	void Update(class Renderer* renderer);
	void Draw(class Renderer* renderer);

	class ParticleSystem* GetParticleSystem() { return mParticleSystems[0]; }

private:
	struct SystemConstant
	{
		float mDeltaTime;
		float mRandom;
		float padding[2];
	};

	struct CameraConstant
	{
		DirectX::XMMATRIX mViewMatrix;
		DirectX::XMMATRIX mProjectionMatrix;
	};

	std::vector<class ParticleSystem*> mParticleSystems;

	class ComputeShader* mParticleInitShader;
	class ComputeShader* mParticleEmitShader;
	class ComputeShader* mParticleUpdateShader;
	class VertexShader* mParticleVertexShader;
	class PixelShader* mParticlePixelShader;
	class GeometryShader* mParticleGeometryShader;

	ComputeConstantBuffer<SystemConstant>* mComputeCBufferSystem;
	VertexConstantBuffer<CameraConstant>* mVertexCBufferCamera;

	class BaseScene* mScene;
};

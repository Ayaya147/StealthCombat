#pragma once
#include <DirectXMath.h>
#include <d3d11.h>
#include <vector>

template<class C>
class ComputeConstantBuffer;
template<class C>
class VertexConstantBuffer;
template<class C>
class GeometryConstantBuffer;

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
		float mScreenWidth;
		float mScreenHeight;
		float mDeltaTime;
		float mTime;
		float mFPS;
		float mD1;
		float mD2;
		float mD3;
	};

	struct CameraConstant
	{
		DirectX::XMMATRIX mViewMatrix;
		DirectX::XMMATRIX mProjectionMatrix;
		DirectX::XMMATRIX mInvProjectionMatrix;
		DirectX::XMMATRIX mInvViewMatrix;
		DirectX::XMFLOAT4 mCameraPosition;
		DirectX::XMFLOAT4 mCameraDir;
	};

	std::vector<class ParticleSystem*> mParticleSystems;

	class ComputeShader* mParticleInitShader;
	class ComputeShader* mParticleEmitShader;
	class ComputeShader* mParticleUpdateShader;
	class VertexShader* mParticleVertexShader;
	class PixelShader* mParticlePixelShader;
	class GeometryShader* mParticleGeometryShader;

	ComputeConstantBuffer<SystemConstant>* mComputeCBufferSystem;
	ComputeConstantBuffer<CameraConstant>* mComputeCBufferCamera;
	VertexConstantBuffer<CameraConstant>* mVertexCBufferCamera;
	GeometryConstantBuffer<SystemConstant>* mGeometryCBufferSystem;

	class BaseScene* mScene;
	DirectX::XMMATRIX mVirtualViewMatrix;
};

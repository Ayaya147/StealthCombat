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

	void Update(class Renderer* renderer);
	void Draw(class Renderer* renderer);

private:
	struct SystemConstant
	{
		float screenWidth;
		float screenHeight;
		float deltaTime;
		float time;
		float fps;
		float d1;
		float d2;
		float d3;
	};

	struct CameraConstant
	{
		DirectX::XMMATRIX viewMatrix;
		DirectX::XMMATRIX projectionMatrix;
		DirectX::XMMATRIX invProjectionMatrix;
		DirectX::XMMATRIX invViewMatrix;
		DirectX::XMFLOAT4 cameraPosition;
		DirectX::XMFLOAT4 cameraDir;
	};

	class ComputeShader* mParticleInitShader;
	class ComputeShader* mParticleEmitShader;
	class ComputeShader* mParticleUpdateShader;
	class VertexShader* mParticleVertexShader;
	class PixelShader* mParticlePixelShader;
	class GeometryShader* mParticleGeometryShader;

	ComputeConstantBuffer<SystemConstant>* mComputeCBufferSystem;
	ComputeConstantBuffer<CameraConstant>* mComputeCBufferCamera;

	
	class Renderer* mRenderer;
	bool mIsInit;
};

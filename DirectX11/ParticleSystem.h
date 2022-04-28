#pragma once
#include <DirectXMath.h>
#include <d3d11.h>


struct VERTEX_3D_PARTICLE
{
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT2 TexCoord;
};
//今回使うパーティクルの資料例
struct ParticleCompute {
	// 座標
	DirectX::XMFLOAT3 pos;
	// 速度
	DirectX::XMFLOAT3 vel;
	// ライフ
	float life;
};


class ParticleSystem
{
public:
	ParticleSystem(class Renderer* renderer);
	~ParticleSystem();

	void Update(class Renderer* renderer);
	void Draw(class Renderer* renderer);

private:
	int mParticleAmount;
	ID3D11ComputeShader* mComputeShader;
	// パーティクル
	ParticleCompute* mparticle;
	// バッファ
	ID3D11Buffer* mVertexBuffer;
	ID3D11Buffer* mpParticleBuffer;
	ID3D11Buffer* mpResultBuffer;
	ID3D11Buffer* mpPositionBuffer;
	// SRV
	ID3D11ShaderResourceView* mpParticleSRV;
	ID3D11ShaderResourceView* mpPositionSRV;
	// UAV
	ID3D11UnorderedAccessView* mpResultUAV;

	class Renderer* mRenderer;
};

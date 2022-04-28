#pragma once
#include <DirectXMath.h>
#include <d3d11.h>


struct VERTEX_3D_PARTICLE
{
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT2 TexCoord;
};
//����g���p�[�e�B�N���̎�����
struct ParticleCompute {
	// ���W
	DirectX::XMFLOAT3 pos;
	// ���x
	DirectX::XMFLOAT3 vel;
	// ���C�t
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
	// �p�[�e�B�N��
	ParticleCompute* mparticle;
	// �o�b�t�@
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

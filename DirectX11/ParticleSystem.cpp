#include "ParticleSystem.h"
#include "Renderer.h"

using namespace DirectX;

#define Size 100
#define Amount 10000

ParticleSystem::ParticleSystem(Renderer* renderer)
	:
	mRenderer(renderer)
{
	VERTEX_3D_PARTICLE vertex[4];
	vertex[0].Position = { -Size, Size, 0.0f };

	vertex[0].TexCoord = { 0.0f, 0.0f };
	vertex[1].Position = { Size, Size, 0.0f };
	vertex[1].TexCoord = { 1.0f, 0.0f };
	vertex[2].Position = { -Size, -Size, 0.0f };
	vertex[2].TexCoord = { 0.0f, 1.0f };
	vertex[3].Position = { Size, -Size, 0.0f };
	vertex[3].TexCoord = { 1.0f, 1.0f };

	//�p�[�e�B�N����������(Amount�͍ő吔)
	mParticleAmount = Amount;
	mparticle = new ParticleCompute[Amount];

	//�p�[�e�B�N���̎��������i�Œ肩�����_���Ƃ������ō��j
	for (int i = 0; i < Amount; i++) {
		mparticle[i].vel = XMFLOAT3(0, 1, 0); // ���x
		mparticle[i].life = 300.0f; // ���C�t�i�t���[���j
		mparticle[i].pos = XMFLOAT3(0, 0, 0); // ���W
	}

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D_PARTICLE) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	// ID3D11Device
	mRenderer->GetDevice()->CreateBuffer(&bd, &sd, &mVertexBuffer);

	/*
	�o�b�t�@����
	D3D11_BUFFER_DESC�̐ݒ�F
 �@ desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.ByteWidth = elementSize * count;
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.StructureByteStride = elementSize;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	*/
	mRenderer->CreateStructuredBuffer(sizeof(ParticleCompute), (UINT)mParticleAmount, nullptr, &mpParticleBuffer);
	mRenderer->CreateStructuredBuffer(sizeof(XMFLOAT3), (UINT)mParticleAmount, nullptr, &mpPositionBuffer)
		mRenderer->CreateStructuredBuffer(sizeof(ParticleCompute), (UINT)mParticleAmount, nullptr, &mpResultBuffer);

	// SRV����
	mRenderer->CreateBufferSRV(mpParticleBuffer, &mpParticleSRV);
	mRenderer->CreateBufferSRV(mpPositionBuffer, &mpPositionSRV);
	// UAV����
	mRenderer->CreateBufferUAV(mpResultBuffer, &mpResultUAV);

	// �R���s���[�g�V�F�[�_�[�쐬
	{
		FILE* file;
		long int fsize;

		file = fopen("ParticleCS", "rb");
		fsize = _filelength(_fileno(file));
		unsigned char* buffer = new unsigned char[fsize];
		fread(buffer, fsize, 1, file);
		fclose(file);

		mRenderer->GetDevice()->CreateComputeShader(buffer, fsize, nullptr, &mComputeShader);

		delete[] buffer;
	}
}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::Update(Renderer* renderer)
{
	// �p�[�e�B�N���̎������o�b�t�@�ɓ����
	D3D11_MAPPED_SUBRESOURCE subRes;
	mRenderer->GetContext()->Map(mpParticleBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subRes);
	ParticleCompute* pBufType = (ParticleCompute*)subRes.pData;
	memcpy(subRes.pData, mparticle, sizeof(ParticleCompute) * mParticleAmount);
	mRenderer->GetContext()->Unmap(mpParticleBuffer, 0);

	//�@�R���s���[�g�V�F�[�_�[���s
	ID3D11ShaderResourceView* pSRVs[1] = { mpParticleSRV };
	mRenderer->GetContext()->CSSetShaderResources(0, 1, pSRVs);
	mRenderer->GetContext()->CSSetShader(&mComputeShader, nullptr, 0);
	mRenderer->GetContext()->CSSetUnorderedAccessViews(0, 1, &mpResultUAV, 0);
	mRenderer->GetContext()->Dispatch(256, 1, 1);

	// �߂����v�Z���ʂ��o�b�t�@�ɓ����
	ID3D11Buffer* pBufDbg = mRenderer->CreateAndCopyToBuffer(mpResultBuffer);
	D3D11_MAPPED_SUBRESOURCE subRes;
	mRenderer->GetContext()->Map(pBufDbg, 0, D3D11_MAP_READ, 0, &subRes);
	ParticleCompute* pBufType = (ParticleCompute*)subRes.pData;
	memcpy(mparticle, pBufType, sizeof(ParticleCompute) * mParticleAmount);
	mRenderer->GetContext()->Unmap(pBufDbg, 0);
	pBufDbg->Release();

	// ���W�����W�o�b�t�@�ɓ����(���_�V�F�[�_�[�Ŏg��)
	mRenderer->GetContext()->Map(mpPositionBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subRes);
	D3DXVECTOR3* pBufType = (D3DXVECTOR3*)subRes.pData;
	for (int v = 0; v < mParticleAmount; v++) {
		pBufType[v] = mparticle[v].pos;
	}
	mRenderer->GetContext()->Unmap(mpPositionBuffer, 0);
}

void ParticleSystem::Draw(Renderer* renderer)
{
	Camera* camera = Application::GetScene()->GetGameObject<Camera>(CameraLayer);
	D3DXMATRIX view = camera->GetViewMatrix();
	D3DXMATRIX invView;
	D3DXMatrixInverse(&invView, NULL, &view);
	invView._41 = 0.0f;
	invView._42 = 0.0f;
	invView._43 = 0.0f;

	// ���[���h���W�A�X�P�[���Ȃǂ̏���
	D3DXMATRIX world, scale, trans;
	D3DXMatrixScaling(&scale, Scale.x, Scale.y, Scale.z);
	D3DXMatrixTranslation(&trans, Position.x, Position.y, Position.z);
	world = scale * invView * trans;
	mRenderer->SetWorldMatrix(&world);

	// �C���v�b�g���C�A�E�g�ݒ�Ȃ�
	mRenderer->SetInputLayout(1);
	UINT stride = sizeof(VERTEX_3D_PARTICLE);
	UINT offset = 0;
	// VS�APS�V�F�[�_�[�ݒ�
	Shader::Use(SHADER_TYPE_VSPS::Particle);
	// �`��
	mRenderer->GetContext()->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	//mRenderer->GetContext()->PSSetShaderResources(0, 1, &mTexture); // �e�N�X�`���ݒ�i����΁j
	mRenderer->GetContext()->VSSetShaderResources(2, 1, &mpPositionSRV); // VS�ɓ������W�ݒ�
	mRenderer->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	mRenderer->GetContext()->DrawInstanced(4, mParticleAmount, 0, 0);
	// �C���v�b�g���C�A�E�g�ݒ�
	mRenderer->SetInputLayout(0);
}

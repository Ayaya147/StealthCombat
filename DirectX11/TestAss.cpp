#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "TestAss.h"
#include <wrl.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <vector>
#include "DxException.h"
#include "Renderer.h"

namespace wrl = Microsoft::WRL;
namespace dx = DirectX;


TestAss::TestAss(Renderer* renderer)
	:
	mRenderer(renderer)
{
	float scale = 0.1f;

	struct Vertex
	{
		dx::XMFLOAT3 pos;
		dx::XMFLOAT3 n;
	};

	Assimp::Importer imp;
	const auto pModel = imp.ReadFile("Assets\\Models\\player.obj",
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices
	);
	const auto pMesh = pModel->mMeshes[0];

	std::vector<Vertex> vertices;
	vertices.reserve(pMesh->mNumVertices);
	for (unsigned int i = 0; i < pMesh->mNumVertices; i++)
	{
		vertices.push_back({
			{ pMesh->mVertices[i].x * scale,pMesh->mVertices[i].y * scale,pMesh->mVertices[i].z * scale },
			*reinterpret_cast<dx::XMFLOAT3*>(&pMesh->mNormals[i])
			});
	}

	std::vector<unsigned short> indices;
	indices.reserve(pMesh->mNumFaces * 3);
	for (unsigned int i = 0; i < pMesh->mNumFaces; i++)
	{
		const auto& face = pMesh->mFaces[i];
		assert(face.mNumIndices == 3);
		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}
	mNum = (unsigned int)indices.size();

	wrl::ComPtr<ID3D11Buffer> vertexBuffer;
	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.ByteWidth = UINT(sizeof(Vertex) * vertices.size());
	bd.StructureByteStride = sizeof(Vertex);

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vertices.data();
	ThrowIfFailed(mRenderer->GetDevice()->CreateBuffer(&bd, &sd, &vertexBuffer));

	const UINT stride = sizeof(Vertex);
	const UINT offset = 0;
	mRenderer->GetContext()->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

	wrl::ComPtr<ID3D11Buffer> indexBuffer;
	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.ByteWidth = UINT(mNum * sizeof(unsigned short));
	ibd.StructureByteStride = sizeof(unsigned short);
	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = indices.data();
	ThrowIfFailed(mRenderer->GetDevice()->CreateBuffer(&ibd, &isd, &indexBuffer));

	mRenderer->GetContext()->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);

	struct PointLightCBuf
	{
		alignas(16) DirectX::XMFLOAT3 pos;
		alignas(16) DirectX::XMFLOAT3 ambient;
		alignas(16) DirectX::XMFLOAT3 diffuseColor;
		float diffuseIntensity;
		float attConst;
		float attLin;
		float attQuad;
	}cbData;

	cbData = {
		{ 0.0f,0.0f,-5.0f },
		{ 0.05f,0.05f,0.05f },
		{ 1.0f,1.0f,1.0f },
		1.0f,
		1.0f,
		0.045f,
		0.0075f,
	};

	wrl::ComPtr<ID3D11Buffer> pConstantBuffer;
	D3D11_BUFFER_DESC cbd;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DEFAULT;
	cbd.CPUAccessFlags = 0u;
	cbd.MiscFlags = 0u;
	cbd.ByteWidth = sizeof(cbData);
	cbd.StructureByteStride = 0u;
	D3D11_SUBRESOURCE_DATA csd = {};
	csd.pSysMem = &cbData;
	ThrowIfFailed(mRenderer->GetDevice()->CreateBuffer(&cbd, &csd, &pConstantBuffer));

	// bind constant buffer to pixel shader
	mRenderer->GetContext()->PSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());



	// create pixel shader
	wrl::ComPtr<ID3D11PixelShader> pPixelShader;
	wrl::ComPtr<ID3DBlob> pBlob;
	ThrowIfFailed(D3DReadFileToBlob(L"ShaderBins/PhongPS.cso", &pBlob));
	ThrowIfFailed(mRenderer->GetDevice()->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader));

	// bind pixel shader
	mRenderer->GetContext()->PSSetShader(pPixelShader.Get(), nullptr, 0u);


	// create vertex shader
	wrl::ComPtr<ID3D11VertexShader> pVertexShader;
	ThrowIfFailed(D3DReadFileToBlob(L"ShaderBins/PhongVS.cso", &pBlob));
	ThrowIfFailed(mRenderer->GetDevice()->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader));

	// bind vertex shader
	mRenderer->GetContext()->VSSetShader(pVertexShader.Get(), nullptr, 0u);


	wrl::ComPtr<ID3D11InputLayout> pInputLayout;
	const D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "Normal",0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};
	ThrowIfFailed(mRenderer->GetDevice()->CreateInputLayout(
		ied, (UINT)std::size(ied),
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		&pInputLayout
	));

	mRenderer->GetContext()->IASetInputLayout(pInputLayout.Get());

	mRenderer->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	struct PSMaterialConstant
	{
		DirectX::XMFLOAT3 color;
		float specularIntensity = 0.6f;
		float specularPower = 30.0f;
		float padding[3];
	} pmc;
	pmc.color = DirectX::XMFLOAT3{1.0f,0.0f,0.0f};

	wrl::ComPtr<ID3D11Buffer> pConstantBuffer2;
	D3D11_BUFFER_DESC cbd2;
	cbd2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd2.Usage = D3D11_USAGE_DEFAULT;
	cbd2.CPUAccessFlags = 0u;
	cbd2.MiscFlags = 0u;
	cbd2.ByteWidth = sizeof(pmc);
	cbd2.StructureByteStride = 0u;
	D3D11_SUBRESOURCE_DATA csd2 = {};
	csd2.pSysMem = &pmc;
	ThrowIfFailed(mRenderer->GetDevice()->CreateBuffer(&cbd2, &csd2, &pConstantBuffer2));


	mRenderer->GetContext()->PSSetConstantBuffers(1u, 1u, pConstantBuffer2.GetAddressOf());

}

void TestAss::Draw()
{
	mAngle += 0.02f;

	struct ConstantBuffer
	{
		DirectX::XMMATRIX modelViewProj;
		DirectX::XMMATRIX model;
	};

	const auto pos = dx::XMVector3Transform(
		dx::XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f),
		dx::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f)
	);

	DirectX::XMMATRIX camera = dx::XMMatrixLookAtLH(pos, dx::XMVectorZero(),dx::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
	) * dx::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);

	const ConstantBuffer cb =
	{		
		dx::XMMatrixTranspose(
			dx::XMMatrixRotationZ(mAngle) *
			dx::XMMatrixRotationX(mAngle) *
			dx::XMMatrixTranslation(0.0f,0.0f,5.0f) *
			camera
		),

		dx::XMMatrixTranspose(
			dx::XMMatrixRotationZ(mAngle) *
			dx::XMMatrixRotationX(mAngle) *
			dx::XMMatrixTranslation(0.0f,0.0f,5.0f) *
			camera *
			dx::XMMatrixPerspectiveLH(1,9.0f / 16.0f,0.5f,100.0f)
		),
	};
	wrl::ComPtr<ID3D11Buffer> pConstantBuffer;
	D3D11_BUFFER_DESC cbd;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0u;
	cbd.ByteWidth = sizeof(cb);
	cbd.StructureByteStride = 0u;
	D3D11_SUBRESOURCE_DATA csd = {};
	csd.pSysMem = &cb;
	ThrowIfFailed(mRenderer->GetDevice()->CreateBuffer(&cbd, &csd, &pConstantBuffer));

	mRenderer->GetContext()->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());

	mRenderer->GetContext()->DrawIndexed((UINT)mNum, 0u, 0u);
}

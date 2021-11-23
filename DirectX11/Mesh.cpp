#include <DirectXMath.h>
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "Mesh.h"
#include "Renderer.h"
#include "IndexBuffer.h"
#include "InputLayout.h"
#include "PixelShader.h"
#include "Topology.h"
#include "VertexBuffer.h"
#include "VertexShader.h"

namespace wrl = Microsoft::WRL;
namespace dx = DirectX;

Mesh::Mesh(const std::string& fileName, Renderer* renderer)
	:
	mRenderer(renderer),
	mFileName(fileName)
{
	float scale = 1.0f;

	struct Vertex
	{
		dx::XMFLOAT3 pos;
		dx::XMFLOAT3 n;
	};

	Assimp::Importer imp;
	const auto pScene = imp.ReadFile(fileName,
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_ConvertToLeftHanded |
		aiProcess_GenNormals |
		aiProcess_CalcTangentSpace
	);
	const auto pMesh = pScene->mMeshes[0];

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

	const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "Normal",0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};

	VertexShader* vs = new VertexShader(renderer, L"ShaderBins/PhongVS.cso");
	mIndexBuffer = new IndexBuffer(renderer, indices);

	AddBind(new VertexBuffer(renderer, vertices));
	AddBind(mIndexBuffer);
	AddBind(vs);
	AddBind(new PixelShader(renderer, L"ShaderBins/PhongPS.cso"));
	AddBind(new InputLayout(renderer, ied, vs));
	AddBind(new Topology(renderer, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

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
		{ 0.0f,0.0f,0.0f },
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
	ThrowIfFailed(renderer->GetDevice()->CreateBuffer(&cbd, &csd, &pConstantBuffer));
	renderer->GetContext()->PSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());

	struct PSMaterialConstant
	{
		DirectX::XMFLOAT3 color;
		float specularIntensity = 0.6f;
		float specularPower = 30.0f;
		float padding[3];
	} pmc;
	pmc.color = DirectX::XMFLOAT3{ 1.0f,0.0f,0.0f };

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
	ThrowIfFailed(renderer->GetDevice()->CreateBuffer(&cbd2, &csd2, &pConstantBuffer2));
	renderer->GetContext()->PSSetConstantBuffers(1u, 1u, pConstantBuffer2.GetAddressOf());
}

Mesh::~Mesh()
{
}

void Mesh::Bind(Renderer* renderer)
{
	for (auto b : mBinds)
	{
		b->Bind(renderer);
	}
}

void Mesh::AddBind(Bindable* bind)
{
	mBinds.emplace_back(bind);
}

unsigned int Mesh::GetIndicesNum() const
{
	return mIndexBuffer->GetCount();
}

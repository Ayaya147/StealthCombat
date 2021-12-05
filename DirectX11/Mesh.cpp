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
#include "ConstantBuffer.h"

namespace wrl = Microsoft::WRL;
namespace dx = DirectX;

Mesh::Mesh(Renderer* renderer, const std::string& fileName, const std::wstring& shaderName)
	:
	mRenderer(renderer),
	mFileName(fileName)
{
	float scale = 0.1f;

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

	std::wstring VSName = L"ShaderBins/" + shaderName + L"VS.cso";
	std::wstring PSName = L"ShaderBins/" + shaderName + L"PS.cso";

	VertexShader* vs = new VertexShader(renderer, VSName);
	mIndexBuffer = new IndexBuffer(renderer, indices);

	//struct PSMaterialConstant
	//{
	//	DirectX::XMFLOAT3 color;
	//	float specularIntensity = 0.6f;
	//	float specularPower = 30.0f;
	//	float padding[3];
	//} pmc;
	//pmc.color = DirectX::XMFLOAT3{ 1.0f,0.0f,0.0f };

	AddBind(new VertexBuffer(renderer, vertices));
	AddBind(mIndexBuffer);
	AddBind(vs);
	AddBind(new PixelShader(renderer, PSName));
	AddBind(new InputLayout(renderer, ied, vs));
	AddBind(new Topology(renderer, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	//AddBind(new PixelConstantBuffer<PSMaterialConstant>(renderer, pmc, 0));
}

Mesh::~Mesh()
{
	for (auto b : mBinds)
	{
		delete b;
	}
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

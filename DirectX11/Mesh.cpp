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
#include "Sampler.h"
#include "Texture.h"
#include "Blender.h"

namespace dx = DirectX;

Mesh::Mesh(const std::string& fileName)
	:
	mFileName("Assets\\Models\\" + fileName + ".obj")
{
}

Mesh::~Mesh()
{
	for (auto b : mBinds)
	{
		delete b;
	}
	mBinds.clear();
}

void Mesh::ParseMesh(Renderer* renderer, const std::string& fileName, const std::wstring& shaderName, bool textured)
{
	Assimp::Importer imp;
	const auto pScene = imp.ReadFile(mFileName,
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_ConvertToLeftHanded |
		aiProcess_GenNormals |
		aiProcess_CalcTangentSpace
	);
	const auto pMesh = pScene->mMeshes[0];

	std::vector<Vertex> vertices;
	mVerticesCount = pMesh->mNumVertices;
	vertices.reserve(mVerticesCount);
	for (int i = 0; i < mVerticesCount; i++)
	{
		vertices.push_back({
			*reinterpret_cast<dx::XMFLOAT3*>(&pMesh->mVertices[i]),
			*reinterpret_cast<dx::XMFLOAT3*>(&pMesh->mNormals[i]),
			*reinterpret_cast<dx::XMFLOAT2*>(&pMesh->mTextureCoords[0][i])
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
		{ "TexCoord",0,DXGI_FORMAT_R32G32_FLOAT,0,24,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};

	std::wstring VSName = L"ShaderBins\\" + shaderName + L"VS.cso";
	std::wstring PSName = L"ShaderBins\\" + shaderName + L"PS.cso";
	std::string texName = "Assets\\Models\\" + fileName + ".png";

	VertexShader* vs = new VertexShader(renderer, VSName);
	mIndexBuffer = new IndexBuffer(renderer, indices);

	AddBind(new VertexBuffer(renderer, vertices));
	AddBind(mIndexBuffer);
	AddBind(vs);
	AddBind(new PixelShader(renderer, PSName));
	AddBind(new InputLayout(renderer, ied, vs));
	AddBind(new Topology(renderer, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	if (textured)
	{
		AddBind(new Texture(renderer, texName));
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

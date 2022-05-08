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
	mFileName("Asset\\Model\\" + fileName + ".obj"),
	mIsMeshParsed(false)
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
	if (!mIsMeshParsed)
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
			{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,24,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};

		std::wstring VSName = L"ShaderBin\\" + shaderName + L"VS.cso";
		std::wstring PSName = L"ShaderBin\\" + shaderName + L"PS.cso";
		std::string texName = "Asset\\Model\\" + fileName + ".png";

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

		mIsMeshParsed = true;
	}
}

void Mesh::ParsePlaneMesh(Renderer* renderer, const std::string& fileName, const std::wstring& shaderName, int n, float size, bool textured)
{
	if (!mIsMeshParsed)
	{
		mVerticesCount = n * n;

		std::vector<Vertex> vertices;
		vertices.reserve(mVerticesCount);
		for (int z = 0; z < n; z++)
		{
			for (int x = 0; x < n; x++)
			{
				vertices.push_back({
					dx::XMFLOAT3{-size / 2.0f * (n - 1) + size * x, 0.0f ,size / 2.0f * (n - 1) - size * z},
					dx::XMFLOAT3{0.0f,1.0f,0.0f},
					dx::XMFLOAT2{1.0f*x,1.0f*z}
				});
			}
		}

		std::vector<unsigned short> indices;
		indices.reserve(2 * n * n - 4);
		for (int y = 0; y < n - 1; y++)
		{
			for (int x = 0; x < n; x++)
			{
				indices.push_back(n + x + n * y);
				indices.push_back(0 + x + n * y);
			}

			if (y < n - 2)
			{
				indices.push_back(n - 1 + n * y);
				indices.push_back(n * 2 + n * y);
			}
		}

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,24,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};

		std::wstring VSName = L"ShaderBin\\" + shaderName + L"VS.cso";
		std::wstring PSName = L"ShaderBin\\" + shaderName + L"PS.cso";
		std::string texName = "Asset\\Model\\" + fileName + ".png";

		VertexShader* vs = new VertexShader(renderer, VSName);
		SetIndexBuffer(new IndexBuffer(renderer, indices));

		AddBind(new VertexBuffer(renderer, vertices));
		AddBind(GetIndexBuffer());
		AddBind(vs);
		AddBind(new PixelShader(renderer, PSName));
		AddBind(new InputLayout(renderer, ied, vs));
		AddBind(new Topology(renderer, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP));
		if (textured)
		{
			AddBind(new Texture(renderer, texName));
		}

		mIsMeshParsed = true;
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

#include "PlaneMesh.h"
#include <DirectXMath.h>
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

void PlaneMesh::ParseMesh(Renderer* renderer, const std::string& fileName, const std::wstring& shaderName, int n, float size)
{
	SetVerticesCount(n * n);

	std::vector<PlaneVertex> vertices;
	vertices.reserve(GetVerticesCount());
	for (int z = 0; z < n; z++)
	{
		for (int x = 0; x < n; x++)
		{
			vertices.push_back({
				dx::XMFLOAT3{-size / 2.0f * (n-1) + size * x, 0.0f ,size / 2.0f * (n-1) - size * z},
				dx::XMFLOAT3{0.0f,1.0f,0.0f},
				dx::XMFLOAT2{x*1.0f, z*1.0f},
				dx::XMFLOAT2{x*1.0f, z*1.0f}
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
		{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "Normal",0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "Tex0Coord",0,DXGI_FORMAT_R32G32_FLOAT,0,24,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "Tex1Coord",0,DXGI_FORMAT_R32G32_FLOAT,0,32,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};

	std::wstring VSName = L"ShaderBins\\" + shaderName + L"VS.cso";
	std::wstring PSName = L"ShaderBins\\" + shaderName + L"PS.cso";
	std::string texName1 = "Assets\\Models\\normal1.png";
	std::string texName2 = "Assets\\Models\\normal2.png";
	std::string texName = "Assets\\Models\\" + fileName + ".png";

	VertexShader* vs = new VertexShader(renderer, VSName);
	SetIndexBuffer(new IndexBuffer(renderer, indices));

	AddBind(new VertexBuffer(renderer, vertices));
	AddBind(GetIndexBuffer());
	AddBind(vs);
	AddBind(new PixelShader(renderer, PSName));
	AddBind(new InputLayout(renderer, ied, vs));
	AddBind(new Topology(renderer, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP));
	AddBind(new Texture(renderer, texName1, 0));
	AddBind(new Texture(renderer, texName2, 1));
}

#pragma once
#include "Mesh.h"
#include <DirectXMath.h>

class PlaneMesh : public Mesh
{
public:
	struct PlaneVertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 n;
		DirectX::XMFLOAT2 tc0;
		DirectX::XMFLOAT2 tc1;
	};

	using Mesh::Mesh;
	void ParseMesh(class Renderer* renderer, const std::string& fileName, const std::wstring& shaderName, int n, float size);
};

#pragma once
#include "Mesh.h"
#include <DirectXMath.h>

class PlaneMesh : public Mesh
{
public:
	struct PlaneVertex
	{
		DirectX::XMFLOAT3 pos;
	};

	using Mesh::Mesh;
	void ParseMesh(class Renderer* renderer, const std::string& fileName, const std::wstring& shaderName, int n, float size);
};

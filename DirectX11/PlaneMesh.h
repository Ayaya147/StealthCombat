#pragma once
#include "Mesh.h"
#include <DirectXMath.h>

struct PlaneVertex
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 n;
	DirectX::XMFLOAT2 tc0;
	DirectX::XMFLOAT2 tc1;
};

class PlaneMesh : public Mesh
{
public:
	using Mesh::Mesh;

	void ParseMesh(class Renderer* renderer, const std::string& fileName, const std::wstring& shaderName, int n, float size);
	int GetVerticesCount() const { return mVerticesCount; }

private:
	int mVerticesCount;
};

#pragma once
#include <string>
#include <vector>
#include <DirectXMath.h>

class Mesh
{
public:
	struct Vertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 n;
		DirectX::XMFLOAT2 tc;
	};

	Mesh(class Renderer* renderer, const std::string& fileName, const std::wstring& shaderName);
	virtual ~Mesh();

	void ParseMesh(class Renderer* renderer, const std::string& fileName, const std::wstring& shaderName);
	void Bind(class Renderer* renderer);
	void AddBind(class Bindable* bind);

	unsigned int GetIndicesNum() const;
	const std::string& GetFileName() const { return mFileName; }
	const std::vector<class Bindable*>& GetBindables() const { return mBinds; }
	class IndexBuffer* GetIndexBuffer() const { return mIndexBuffer; }
	int GetVerticesCount() const { return mVerticesCount; }
	void SetIndexBuffer(class IndexBuffer* indexBuffer) { mIndexBuffer = indexBuffer; }
	void SetVerticesCount(int count) { mVerticesCount = count; }

private:
	std::vector<class Bindable*> mBinds;
	class IndexBuffer* mIndexBuffer;
	std::string mFileName;
	int mVerticesCount;
};

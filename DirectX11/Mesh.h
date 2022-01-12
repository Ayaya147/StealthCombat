#pragma once
#include <string>
#include <vector>

struct Vertex
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 n;
	DirectX::XMFLOAT3 tangent;
	DirectX::XMFLOAT3 bitangent;
	DirectX::XMFLOAT2 tc0;
	DirectX::XMFLOAT2 tc1;
};

class Mesh
{
public:
	Mesh(class Renderer* renderer, const std::string& fileName, const std::wstring& shaderName, int test = 1);
	virtual ~Mesh();

	void Bind(class Renderer* renderer);
	void AddBind(class Bindable* bind);

	unsigned int GetIndicesNum() const;
	const std::string& GetFileName() const { return mFileName; }
	const std::vector<class Bindable*>& GetBindables() const { return mBinds; }
	int GetCount() { return mCount; }


private:
	//std::vector<Texture*> mTextures;
	std::vector<class Bindable*> mBinds;
	class IndexBuffer* mIndexBuffer;
	std::string mFileName;
	int mCount;
};

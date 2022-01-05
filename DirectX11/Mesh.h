#pragma once
#include <string>
#include <vector>

class Renderer;
class Bindable;
class Texture;
class IndexBuffer;

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
	Mesh(Renderer* renderer, const std::string& fileName, const std::wstring& shaderName);
	virtual ~Mesh();

	void Bind(Renderer* renderer);
	void AddBind(Bindable* bind);

	unsigned int GetIndicesNum() const;
	const std::string& GetFileName() const { return mFileName; }
	const std::vector<Bindable*>& GetBindables() const { return mBinds; }
	int GetCount() { return mCount; }


private:
	//std::vector<Texture*> mTextures;
	std::vector<Bindable*> mBinds;
	IndexBuffer* mIndexBuffer;
	std::string mFileName;
	int mCount;
};

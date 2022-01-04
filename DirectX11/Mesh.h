#pragma once
#include <string>
#include <vector>

class Renderer;
class Bindable;
class Texture;
class IndexBuffer;

class Mesh
{
public:
	Mesh(Renderer* renderer, const std::string& fileName, const std::wstring& shaderName);
	virtual ~Mesh();

	void Bind(Renderer* renderer);
	void AddBind(Bindable* bind);

	unsigned int GetIndicesNum() const;
	const std::string& GetFileName() const { return mFileName; }

private:
	//std::vector<Texture*> mTextures;
	std::vector<Bindable*> mBinds;
	IndexBuffer* mIndexBuffer;
	std::string mFileName;
};

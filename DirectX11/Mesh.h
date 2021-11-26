#pragma once
#include <string>
#include <vector>

class Mesh
{
public:
	Mesh(class Renderer* renderer, const std::string& fileName, const std::wstring& shaderName);
	~Mesh();

	void Bind(class Renderer* renderer);
	void AddBind(class Bindable* bind);

	unsigned int GetIndicesNum() const;
	const std::string& GetFileName() const { return mFileName; }

private:
	std::vector<class Texture*> mTextures;
	std::vector<class Bindable*> mBinds;

	class Renderer* mRenderer;
	class IndexBuffer* mIndexBuffer;
	std::string mFileName;
};

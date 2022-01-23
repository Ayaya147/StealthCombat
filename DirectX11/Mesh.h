#pragma once
#include <string>
#include <vector>

class Mesh
{
public:
	Mesh(class Renderer* renderer, const std::string& fileName, const std::wstring& shaderName);
	virtual ~Mesh();

	void ParseMesh(class Renderer* renderer, const std::string& fileName, const std::wstring& shaderName);
	void Bind(class Renderer* renderer);
	void AddBind(class Bindable* bind);

	unsigned int GetIndicesNum() const;
	const std::string& GetFileName() const { return mFileName; }
	const std::vector<class Bindable*>& GetBindables() const { return mBinds; }
	class IndexBuffer* GetIndexBuffer() const { return mIndexBuffer; }
	void SetIndexBuffer(class IndexBuffer* indexBuffer) { mIndexBuffer = indexBuffer; }

private:
	std::vector<class Bindable*> mBinds;
	class IndexBuffer* mIndexBuffer;
	std::string mFileName;
};

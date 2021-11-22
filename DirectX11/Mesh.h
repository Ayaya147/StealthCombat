#pragma once
#include <string>
#include <vector>

class Mesh
{
public:
	Mesh(const std::string& fileName, class Renderer* renderer);
	~Mesh();

	void Bind(class Renderer* renderer);

	unsigned int GetIndicesNum() const { return mNum; }
	const std::string& GetFileName() const { return mFileName; }

private:
	std::vector<class Texture*> mTextures;
	std::vector<class Bindable*> mBinds;

	class Renderer* mRenderer;
	std::string mFileName;
	unsigned int mNum;
};

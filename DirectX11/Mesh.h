#pragma once
#include <string>
#include <vector>

class Mesh
{
public:
	Mesh(const std::string& fileName, class Renderer* renderer);
	~Mesh();

	unsigned int GetIndicesNum() const { return mNum; }

private:
	std::vector<class Texture*> mTextures;

	unsigned int mNum;
};

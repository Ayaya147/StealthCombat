#pragma once
#include <string>
#include <vector>
#include <memory>

class Mesh
{
public:
	Mesh(const std::string& fileName, class Renderer* renderer);
	~Mesh();

	unsigned int GetIndicesNum() const { return mNum; }

private:
	unsigned int mNum;
};

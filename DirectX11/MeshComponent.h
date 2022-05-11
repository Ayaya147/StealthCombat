#pragma once
#include "Component.h"

class MeshComponent : public Component
{
public:
	MeshComponent(class Actor* owner, class Mesh* mesh);
	~MeshComponent();

	void Draw(class Renderer* renderer);

	class Mesh* GetMesh() const { return mMesh; }
	void SetMesh(class Mesh* mesh) { mMesh = mesh; }

private:
	class Mesh* mMesh;
};

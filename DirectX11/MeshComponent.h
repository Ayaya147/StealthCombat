#pragma once
#include "Component.h"

class Renderer;
class Mesh;

class MeshComponent : public Component
{
public:
	MeshComponent(Actor* owner, class Mesh* mesh);
	~MeshComponent();

	void Draw(Renderer* renderer);

	Mesh* GetMesh() const { return mMesh; }
	void SetMesh(Mesh* mesh) { mMesh = mesh; }

protected:
	Mesh* mMesh;
};

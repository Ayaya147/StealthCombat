#pragma once
#include "Component.h"

class MeshComponent : public Component
{
public:
	MeshComponent(class Actor* owner);
	~MeshComponent();

	virtual void Draw(class Renderer* renderer);
	virtual void SetMesh(class Mesh* mesh) { mMesh = mesh; }

protected:
	class Mesh* mMesh;
};

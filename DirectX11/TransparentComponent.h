#pragma once
#include "Component.h"

class TransparentComponent : public Component
{
public:
	TransparentComponent(class Actor* owner, class Mesh* mesh);
	~TransparentComponent();

	void Draw(class Renderer* renderer);

	class Mesh* GetMesh() const { return mMesh; }
	float GetDistFromCamera() const;
	void SetMesh(class Mesh* mesh) { mMesh = mesh; }

protected:
	class Mesh* mMesh;
};

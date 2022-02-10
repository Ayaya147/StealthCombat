#pragma once
#include "Component.h"

class TransparentComponent : public Component
{
public:
	TransparentComponent(class Actor* owner, class Mesh* mesh, int drawOrder = 100);
	~TransparentComponent();

	void Draw(class Renderer* renderer);

	void SetMesh(class Mesh* mesh) { mMesh = mesh; }
	float GetDistFromCamera() const;
	class Mesh* GetMesh() const { return mMesh; }
	int GetDrawOrder() const { return mDrawOrder; }

protected:
	class Mesh* mMesh;
	int mDrawOrder;
};

#pragma once
#include "Component.h"

class TranslucenceComponent : public Component
{
public:
	TranslucenceComponent(class Actor* owner, class Mesh* mesh, int drawOrder = 100);
	~TranslucenceComponent();

	void Draw(class Renderer* renderer);

	void SetMesh(class Mesh* mesh) { mMesh = mesh; }
	void SetVisible(bool visible) { mIsVisible = visible; }
	class Mesh* GetMesh() const { return mMesh; }
	int GetDrawOrder() const { return mDrawOrder; }
	bool GetIsVisible() const { return mIsVisible; }

private:
	class Mesh* mMesh;
	int mDrawOrder;
	bool mIsVisible;
};

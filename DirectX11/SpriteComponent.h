#pragma once
#include "Component.h"

class SpriteComponent : public Component
{
public:
	SpriteComponent(class Actor* owner, int drawOrder = 100);
	virtual ~SpriteComponent();

	virtual void Draw(class Renderer* renderer);

	void SetTexture(class Texture* tex);
	void SetVisible(bool visible) { mIsVisible = visible; }
	bool GetIsVisible() const { return mIsVisible; }
	int GetDrawOrder() const { return mDrawOrder; }

private:
	class Texture* mTexture;
	int mDrawOrder;
	bool mIsVisible;
};

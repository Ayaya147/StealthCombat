#pragma once
#include "Component.h"

class SpriteComponent : public Component
{
public:
	SpriteComponent(class Actor* owner, class Texture* tex, int drawOrder = 100);
	virtual ~SpriteComponent();

	virtual void Draw(class Renderer* renderer);

	void SetTexture(class Texture* tex);
	void SetVisible(bool visible) { mIsVisible = visible; }
	class Texture* GetTexture() const { return mTexture; }
	bool GetIsVisible() const { return mIsVisible; }
	int GetDrawOrder() const { return mDrawOrder; }

private:
	class Texture* mTexture;
	int mDrawOrder;
	bool mIsVisible;
};

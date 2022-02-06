#pragma once
#include "Component.h"

class SpriteComponent : public Component
{
public:
	SpriteComponent(class Actor* owner, class Texture* tex, int drawOrder = 100);
	~SpriteComponent();

	virtual void Draw(class Renderer* renderer);
	void SetTexture(class Texture* tex);

	int GetDrawOrder() const { return mDrawOrder; }

private:
	class Texture* mTexture;
	int mDrawOrder;
};

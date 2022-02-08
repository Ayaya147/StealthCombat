#pragma once
#include "SpriteComponent.h"
#include <vector>

class NumberSpriteComponent : public SpriteComponent
{
public:
	NumberSpriteComponent(class NumberActor* owner, int drawOrder = 200);

	void Draw(class Renderer* renderer) override;

private:
	std::vector<class Texture*> mTextures;
};

#pragma once
#include "SpriteComponent.h"

class NumberSpriteComponent : public SpriteComponent
{
public:
	NumberSpriteComponent(class NumberActor* owner, class Texture* tex, int drawOrder = 200);

	void Draw(class Renderer* renderer, class VertexBuffer* vertexBuffer) override;
};

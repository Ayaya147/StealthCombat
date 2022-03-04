#pragma once
#include <DirectXMath.h>
#include "Component.h"

class SpriteComponent : public Component
{
public:
	SpriteComponent(class Actor* owner, class Texture* tex, int drawOrder = 100);
	virtual ~SpriteComponent();

	virtual void Draw(class Renderer* renderer, class VertexBuffer* vertexBuffer);

	void SetTexture(class Texture* tex);
	void SetVisible(bool visible) { mIsVisible = visible; }
	void SetColor(DirectX::XMFLOAT4 color) { mColor = color; }
	class Texture* GetTexture() const { return mTexture; }
	const DirectX::XMFLOAT4& GetColor() const { return mColor; }
	bool GetIsVisible() const { return mIsVisible; }
	int GetDrawOrder() const { return mDrawOrder; }

private:
	class Texture* mTexture;
	DirectX::XMFLOAT4 mColor;
	int mDrawOrder;
	bool mIsVisible;
};

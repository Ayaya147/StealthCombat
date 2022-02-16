#pragma once
#include <vector>
#include <DirectXMath.h>

class Minimap
{
public:
	Minimap(class GameScene* game);
	~Minimap();

	void Update(class GameScene* game);

private:
	static constexpr DirectX::XMFLOAT2 mOrigin = { -670.0f, -270.0f };

	std::vector<class SpriteComponent*> mCloudSprites;
	class SpriteComponent* mDirectionSprite;
	float mRadius;
};

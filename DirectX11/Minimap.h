#pragma once
#include <vector>
#include <DirectXMath.h>

class Minimap
{
public:
	Minimap(class GameScene* game);
	~Minimap();

	void Update(class GameScene* game);
	void RemoveEnemySprites();

private:
	static constexpr DirectX::XMFLOAT2 mOrigin = { -670.0f, -270.0f };
	static constexpr float minimapRadius = 180.0f;

	std::vector<class SpriteComponent*> mCloudSprites;
	std::vector<class SpriteComponent*> mEnemySprites;
	std::vector<class SpriteComponent*> mDirectionSprites;
};

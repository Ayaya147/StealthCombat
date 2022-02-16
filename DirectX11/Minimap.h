#pragma once

class Minimap
{
public:
	Minimap(class GameScene* game);
	~Minimap();

	void Update(class GameScene* game);

private:
	class SpriteComponent* mDirectionSprite;
};

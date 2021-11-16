#pragma once
#include "Actor.h"

class PlayerActor : public Actor
{
public:
	PlayerActor(class GameScene* game);

	void UpdateActor() override;
};

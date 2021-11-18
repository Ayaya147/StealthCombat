#pragma once
#include "Actor.h"

class PlayerActor : public Actor
{
public:
	PlayerActor(class AbstractScene* scene);

	void UpdateActor(float deltaTime) override;
};

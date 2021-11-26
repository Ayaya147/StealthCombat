#pragma once
#include <string>
#include "Actor.h"

class PlayerActor : public Actor
{
public:
	PlayerActor(class BaseScene* scene, const std::string& fileName);

	void UpdateActor(float deltaTime) override;

private:
};

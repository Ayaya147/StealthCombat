#pragma once
#include <string>
#include "Actor.h"

class PlayerActor : public Actor
{
public:
	PlayerActor(BaseScene* scene, const std::string& fileName);

	void UpdateActor(float deltaTime) override;

private:
};

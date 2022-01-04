#pragma once
#include "Actor.h"

class PlaneActor : public Actor
{
public:
	PlaneActor(BaseScene* scene);

	void UpdateActor(float deltaTime) override;
	void Bind(Renderer* renderer) override;

private:

};


#pragma once
#include "Actor.h"

class VertexBuffer;

class PlaneActor : public Actor
{
public:
	PlaneActor(BaseScene* scene);
	~PlaneActor();

	void UpdateActor(float deltaTime) override;

private:
	VertexBuffer* mVertexBuffer;
	int mCount;
};

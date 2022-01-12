#pragma once
#include "Actor.h"

class PlaneActor : public Actor
{
public:
	PlaneActor(class BaseScene* scene);
	~PlaneActor();

	void UpdateActor(float deltaTime) override;

private:
	class VertexBuffer* mVertexBuffer;
	int mCount;
};

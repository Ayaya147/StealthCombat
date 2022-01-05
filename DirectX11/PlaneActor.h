#pragma once
#include "Actor.h"

class TransformCBufferDouble;
class VertexBuffer;

class PlaneActor : public Actor
{
public:
	PlaneActor(BaseScene* scene);
	~PlaneActor();

	void UpdateActor(float deltaTime) override;
	void Bind(Renderer* renderer) override;

private:
	TransformCBuffer* mBuffer;
	VertexBuffer* mVertexBuffer;
	int mCount;
};

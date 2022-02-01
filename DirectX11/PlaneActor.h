#pragma once
#include "Actor.h"

template<class C>
class VertexConstantBuffer;

template<class C>
class PixelConstantBuffer;

class PlaneActor : public Actor
{
public:
	PlaneActor(class BaseScene* scene);
	~PlaneActor();

	void UpdateActor(float deltaTime) override;
	void Bind(class Renderer* renderer) override;

private:
	//struct ObjectConstant
	//{
	//	float time;
	//	float padding[3];
	//};

	//VertexConstantBuffer<ObjectConstant>* mObjectCBuffer1;
	//PixelConstantBuffer<ObjectConstant>* mObjectCBuffer2;


	class VertexBuffer* mVertexBuffer;
	int mCount;
};

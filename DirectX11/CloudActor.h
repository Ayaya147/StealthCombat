#pragma once
#include <string>
#include "Actor.h"

template<class C>
class PixelConstantBuffer;

class CloudActor : public Actor
{
public:
	CloudActor(class BaseScene* scene, const std::string& fileName);

	void UpdateActor(float deltaTime) override {};
	void ActorInput() override {};
	void Bind(class Renderer* renderer) override;

private:
	struct ObjectConstant
	{
		DirectX::XMMATRIX mWorldTransformInverse;
	};

	PixelConstantBuffer<ObjectConstant>* mObjectCBuffer;
};

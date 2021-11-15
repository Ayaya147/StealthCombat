#pragma once
#include <vector>
#include <memory>
#include <DirectXMath.h>

class Actor
{
public:
	Actor(class GameScene* game);
	virtual ~Actor();

	void Update();
	void UpdateComponents();
	virtual void UpdateActor();

	void ComputeWorldTransform();
	//const DirectX::XMMATRIX& GetWorldTransform() const { return mWorldTransform; }
	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);

protected:
	DirectX::XMMATRIX mWorldTransform;
	DirectX::XMFLOAT3 mPosition;
	DirectX::XMFLOAT3 mRotation;
	float mScale;

	std::vector<std::shared_ptr<class Component>> mComponents;
	class GameScene* mGame;
};

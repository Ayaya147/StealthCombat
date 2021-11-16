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
	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);

	class GameScene* GetGame() const { return mGame; }
	const DirectX::XMMATRIX& GetWorldTransform() const { return mWorldTransform; }
	const DirectX::XMFLOAT3& GetRotation() const{ return mRotation; }

protected:
	DirectX::XMMATRIX mWorldTransform;
	DirectX::XMFLOAT3 mPosition;
	DirectX::XMFLOAT3 mRotation;
	float mScale;

	std::vector<class Component*> mComponents;
	class GameScene* mGame;
};

#pragma once

class Actor;

class Component
{
public:
	Component(Actor* owner, int updateOrder = 100);
	virtual ~Component();

	virtual void Update(float deltaTime);

	int GetUpdateOrder() const { return mUpdateOrder; }
	Actor* GetOwner() const { return mOwner; }

protected:
	Actor* mOwner;
	int mUpdateOrder;
};

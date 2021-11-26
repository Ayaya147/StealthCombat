#pragma once

class Component
{
public:
	Component(class Actor* owner, int updateOrder = 100);
	virtual ~Component();

	virtual void Update(float deltaTime);

	int GetUpdateOrder() const { return mUpdateOrder; }
	class Actor* GetOwner() const { return mOwner; }

protected:
	class Actor* mOwner;
	int mUpdateOrder;
};

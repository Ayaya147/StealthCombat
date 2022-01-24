#pragma once

class Component
{
public:
	Component(class Actor* owner, int updateOrder = 100);
	virtual ~Component();

	virtual void Update(float deltaTime) {};
	virtual void ProcessInput() {}

	int GetUpdateOrder() const { return mUpdateOrder; }
	class Actor* GetOwner() const { return mOwner; }

private:
	class Actor* mOwner;
	int mUpdateOrder;
};

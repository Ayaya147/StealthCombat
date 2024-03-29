#pragma once
#include "Actor.h"

class NumberActor : public Actor
{
public:
	NumberActor(class BaseScene* scene, float value, int digit);
	virtual ~NumberActor();

	void UpdateActor(float deltaTime) override;
	void ResetPos();

	class NumberSpriteComponent* GetNumberSpriteComp() const { return mNumberSpriteComp; }
	float GetValue() const { return mValue; }
	int GetDigit() const { return mDigit; }
	void SetValue(float value) { mValue = value; }
	void SetOriPosition(const DirectX::XMFLOAT3& pos) { mOriPos = pos; }

private:
	class NumberSpriteComponent* mNumberSpriteComp;
	DirectX::XMFLOAT3 mOriPos;
	float mValue;
	int mDigit;
};

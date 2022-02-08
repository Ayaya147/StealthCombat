#include "NumberActor.h"
#include "GameScene.h"
#include "PlayerActor.h"
#include "NumberSpriteComponent.h"
#include "TransformCBuffer.h"
#include "Renderer.h"

NumberActor::NumberActor(BaseScene* scene, float value, int digit)
	:
	Actor(scene),
	mValue(value),
	mDigit(digit),
	mOriPos(DirectX::XMFLOAT3{0.0f,0.0f,0.0f})
{
	NumberSpriteComponent* nsc = new NumberSpriteComponent(this);
	SetScale(0.6f);
}

NumberActor::~NumberActor()
{
}

void NumberActor::UpdateActor(float deltaTime)
{
	GameScene* scene = dynamic_cast<GameScene*>(GetScene());
	mValue = scene->GetPlayer()->GetForwardSpeed() * 150.0f;
}

void NumberActor::ResetPos()
{
	Actor::SetPosition(mOriPos);
}

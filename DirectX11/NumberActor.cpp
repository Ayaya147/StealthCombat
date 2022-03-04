#include "NumberActor.h"
#include "GameScene.h"
#include "PlayerActor.h"
#include "TransformCBuffer.h"
#include "Renderer.h"
#include "NumberSpriteComponent.h"
#include "Texture.h"

NumberActor::NumberActor(BaseScene* scene, float value, int digit)
	:
	Actor(scene),
	mValue(value),
	mDigit(digit),
	mOriPos(DirectX::XMFLOAT3{0.0f,0.0f,0.0f})
{
	Texture* tex = GetScene()->GetRenderer()->GetTexture("number");
	mNumberSpriteComp = new NumberSpriteComponent(this, tex);
}

NumberActor::~NumberActor()
{
}

void NumberActor::UpdateActor(float deltaTime)
{
}

void NumberActor::ResetPos()
{
	Actor::SetPosition(mOriPos);
}

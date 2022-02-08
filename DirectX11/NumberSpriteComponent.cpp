#include "NumberSpriteComponent.h"
#include <iostream>
#include <string>
#include <DirectXMath.h>
#include "NumberActor.h"
#include "Texture.h"
#include "BaseScene.h"
#include "Renderer.h"

namespace dx = DirectX;

NumberSpriteComponent::NumberSpriteComponent(NumberActor* owner, int drawOrder)
	:
	SpriteComponent(owner, drawOrder)
{
	Renderer* renderer = GetOwner()->GetScene()->GetRenderer();
	for (int i = 0; i < 10; i++)
	{
		std::string fileName = "Assets/Texture/" + std::to_string(i) + ".png";
		mTextures.emplace_back(renderer->GetTexture(fileName));
	}
}

void NumberSpriteComponent::Draw(Renderer* renderer)
{
	auto owner = dynamic_cast<NumberActor*>(GetOwner());
	int value = static_cast<int>(owner->GetValue());
	int digit = owner->GetDigit();
	float dist = 20.0f;
	owner->ResetPos();
	owner->ComputeWorldTransform();

	for (int i = 0; i < digit; i++)
	{
		int n = value % 10;
		//float dist = owner->GetSize().x * owner->GetScale() + 5.0f;

		owner->Bind(renderer, static_cast<float>(mTextures[n]->GetTexWidth()), static_cast<float>(mTextures[n]->GetTexHeight()));
		mTextures[n]->Bind(renderer);
		renderer->GetContext()->Draw(4, 0);

		owner->SetPosition(dx::XMFLOAT3{ owner->GetPosition().x - dist, owner->GetPosition().y, 0.0f });
		owner->ComputeWorldTransform();
		value /= 10;
	}
}

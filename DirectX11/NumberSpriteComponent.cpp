#include "NumberSpriteComponent.h"
#include "NumberActor.h"
#include "Texture.h"
#include "BaseScene.h"
#include "Renderer.h"
#include "VertexBuffer.h"

namespace dx = DirectX;

NumberSpriteComponent::NumberSpriteComponent(NumberActor* owner, Texture* tex, int drawOrder)
	:
	SpriteComponent(owner, tex, drawOrder)
{
}

void NumberSpriteComponent::Draw(Renderer* renderer, VertexBuffer* vertexBuffer)
{
	GetTexture()->Bind(renderer);

	auto owner = dynamic_cast<NumberActor*>(GetOwner());
	int value = static_cast<int>(owner->GetValue());
	int digit = owner->GetDigit();
	owner->ResetPos();
	owner->ComputeWorldTransform();
	float dist = 22.0f;
	dx::XMFLOAT4 color = GetColor();

	for (int i = 0; i < digit; i++)
	{
		int n = value % 10;

		D3D11_MAPPED_SUBRESOURCE msr;
		renderer->GetContext()->Map(vertexBuffer->GetVertexBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
		Renderer::Vertex* vertex = static_cast<Renderer::Vertex*>(msr.pData);

		vertex[0].pos = { -0.5f,-0.5f,0.0f };
		vertex[1].pos = {  0.5f,-0.5f,0.0f };
		vertex[2].pos = { -0.5f, 0.5f,0.0f };
		vertex[3].pos = {  0.5f, 0.5f,0.0f };

		vertex[0].col = color;
		vertex[1].col = color;
		vertex[2].col = color;
		vertex[3].col = color;

		vertex[0].tc = { n*0.1f,0.0f };
		vertex[1].tc = { (n + 1)*0.1f,0.0f };
		vertex[2].tc = { n*0.1f,1.0f };
		vertex[3].tc = { (n + 1)*0.1f,1.0f };

		renderer->GetContext()->Unmap(vertexBuffer->GetVertexBuffer(), 0);		
		vertexBuffer->Bind(renderer);

		owner->Bind(
			renderer,
			static_cast<float>(GetTexture()->GetTexWidth()) / 10.0f,
			static_cast<float>(GetTexture()->GetTexHeight())
		);

		renderer->GetContext()->Draw(4, 0);

		owner->SetPosition(dx::XMFLOAT3{ owner->GetPosition().x - dist, owner->GetPosition().y, 0.0f });
		owner->ComputeWorldTransform();
		value /= 10;
	}
}

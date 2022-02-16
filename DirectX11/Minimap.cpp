#include "Minimap.h"
#include "GameScene.h"
#include "SpriteComponent.h"
#include "PlayerActor.h"
#include "Texture.h"
#include "Renderer.h"

namespace dx = DirectX;

Minimap::Minimap(GameScene* game)
{
	Renderer* renderer = game->GetRenderer();

	Actor* sprite = new Actor(game);
	Texture* tex = renderer->GetTexture("Assets\\Texture\\minimap.png");
	SpriteComponent* sc = new SpriteComponent(sprite);
	sc->SetTexture(tex);
	sprite->SetPosition(dx::XMFLOAT3{ -670.0f, -270.0f, 0.0f });
	sprite->SetScale(0.6f);

	sprite = new Actor(game);
	tex = renderer->GetTexture("Assets\\Texture\\minimap_direction.png");
	mDirectionSprite = new SpriteComponent(sprite);
	mDirectionSprite->SetTexture(tex);
	sprite->SetPosition(dx::XMFLOAT3{ -670.0f, -270.0f, 0.0f });
	sprite->SetScale(0.55f);

	sprite = new Actor(game);
	tex = renderer->GetTexture("Assets\\Texture\\minimap_player.png");
	sc = new SpriteComponent(sprite);
	sc->SetTexture(tex);
	sprite->SetPosition(dx::XMFLOAT3{ -670.0f, -270.0f, 0.0f });
	sprite->SetScale(0.1f);
}

Minimap::~Minimap()
{
}

void Minimap::Update(GameScene* game)
{
	mDirectionSprite->GetOwner()->SetRotation(dx::XMFLOAT3{ 0.0f,0.0f,game->GetPlayer()->GetRotation().y });
}

#include "Minimap.h"
#include "GameScene.h"
#include "SpriteComponent.h"
#include "PlayerActor.h"
#include "CloudActor.h"
#include "Texture.h"
#include "Renderer.h"
#include "XMFloatHelper.h"

namespace dx = DirectX;

Minimap::Minimap(GameScene* game)
	:
	mRadius(500.0f)
{
	Renderer* renderer = game->GetRenderer();

	Actor* sprite = new Actor(game);
	Texture* tex = renderer->GetTexture("minimap");
	SpriteComponent* sc = new SpriteComponent(sprite);
	sc->SetTexture(tex);
	sprite->SetPosition(dx::XMFLOAT3{ mOrigin.x, mOrigin.y, 0.0f });
	sprite->SetScale(0.6f);

	sprite = new Actor(game);
	tex = renderer->GetTexture("minimap_direction");
	mDirectionSprite = new SpriteComponent(sprite);
	mDirectionSprite->SetTexture(tex);
	sprite->SetPosition(dx::XMFLOAT3{ mOrigin.x, mOrigin.y, 0.0f });
	sprite->SetScale(0.55f);

	sprite = new Actor(game);
	tex = renderer->GetTexture("minimap_player");
	sc = new SpriteComponent(sprite,101);
	sc->SetTexture(tex);
	sprite->SetPosition(dx::XMFLOAT3{ mOrigin.x, mOrigin.y, 0.0f });
	sprite->SetScale(0.1f);

	tex = game->GetRenderer()->GetTexture("minimap_cloud");
	std::vector<CloudActor*> clouds = game->GetClouds();
	for (int i = 0; i < clouds.size(); i++)
	{
		Actor* sprite = new Actor(game);
		sprite->SetScale(clouds[i]->GetScale().x / 500.0f);
		sc = new SpriteComponent(sprite);
		mCloudSprites.emplace_back(sc);
		sc->SetTexture(tex);
	}
}

Minimap::~Minimap()
{
}

void Minimap::Update(GameScene* game)
{
	int i = 0;
	std::vector<CloudActor*> clouds = game->GetClouds();
	float angle = -game->GetPlayer()->GetRotation().y;

	for (auto cs : mCloudSprites)
	{
		dx::XMFLOAT3 vec = clouds[i]->GetPosition() - game->GetPlayer()->GetPosition();
		dx::XMFLOAT3 relativePos = vec / (mRadius / minimapRadius);
		dx::XMFLOAT2 pos = { cos(angle)*relativePos.x - sin(angle)* -relativePos.z,
			sin(angle)*relativePos.x + cos(angle)* -relativePos.z
		};

		if ((pos.x >= -minimapRadius && pos.x <= minimapRadius) &&
			(pos.y >= -minimapRadius && pos.y <= minimapRadius))
		{
			cs->SetVisible(true);
			cs->GetOwner()->SetPosition(dx::XMFLOAT3{ pos.x + mOrigin.x, pos.y + mOrigin.y, 0.0f });
		}
		else
		{
			cs->SetVisible(false);
		}
		
		i++;
	}

	mDirectionSprite->GetOwner()->SetRotation(dx::XMFLOAT3{ 0.0f,0.0f,game->GetPlayer()->GetRotation().y });
}

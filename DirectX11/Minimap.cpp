#include "Minimap.h"
#include "GameScene.h"
#include "SpriteComponent.h"
#include "PlayerActor.h"
#include "CloudActor.h"
#include "EnemyActor.h"
#include "Texture.h"
#include "Renderer.h"
#include "XMFloatHelper.h"
#include "DefineConstant.h"

namespace dx = DirectX;

Minimap::Minimap(GameScene* game)
{
	float range = Constant::createRange * 1.2f;
	Renderer* renderer = game->GetRenderer();

	Actor* sprite = new Actor(game);
	Texture* tex = renderer->GetTexture("minimap");
	SpriteComponent* sc = new SpriteComponent(sprite, tex);
	sprite->SetPosition(dx::XMFLOAT3{ mOrigin.x, mOrigin.y, 0.0f });
	sprite->SetScale(0.6f);

	sprite = new Actor(game);
	tex = renderer->GetTexture("minimap_player");
	sc = new SpriteComponent(sprite, tex, 101);
	sprite->SetPosition(dx::XMFLOAT3{ mOrigin.x, mOrigin.y, 0.0f });
	sprite->SetScale(50.0f / range);

	for (int i = 0; i < 4; i++)
	{
		sprite = new Actor(game);
		std::string name = "minimap_direction_" + std::to_string(i);
		tex = renderer->GetTexture(name);
		sc = new SpriteComponent(sprite, tex, 110);
		mDirectionSprites.emplace_back(sc);
		sprite->SetScale(0.4f);
	}

	tex = game->GetRenderer()->GetTexture("minimap_cloud");
	std::vector<CloudActor*> clouds = game->GetClouds();
	for (int i = 0; i < clouds.size(); i++)
	{
		Actor* sprite = new Actor(game);
		sprite->SetScale(clouds[i]->GetScale().x / range * 0.9f);
		sc = new SpriteComponent(sprite, tex);
		mCloudSprites.emplace_back(sc);
	}

	tex = game->GetRenderer()->GetTexture("minimap_enemy");
	std::vector<EnemyActor*> enemies = game->GetEnemies();
	for (int i = 0; i < enemies.size(); i++)
	{
		Actor* sprite = new Actor(game);
		sprite->SetScale(50.0f / range);
		sc = new SpriteComponent(sprite, tex, 101);
		mEnemySprites.emplace_back(sc);
	}
}

Minimap::~Minimap()
{
}

void Minimap::Update(GameScene* game)
{
	// cloud sprites
	int idx = 0;
	float range = Constant::createRange * 1.2f;
	float angle = -game->GetPlayer()->GetRotation().y;
	std::vector<CloudActor*> clouds = game->GetClouds();
	for (auto cs : mCloudSprites)
	{
		dx::XMFLOAT3 vec = clouds[idx]->GetPosition() - game->GetPlayer()->GetPosition();
		dx::XMFLOAT3 relativePos = vec / (range / minimapRadius);
		dx::XMFLOAT2 pos = {
			cos(angle)*relativePos.x - sin(angle)* -relativePos.z,
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
		
		idx++;
	}

	// enemy sprites
	idx = 0;
	std::vector<EnemyActor*> enemies = game->GetEnemies();
	for (auto es : mEnemySprites)
	{
		if (enemies[idx]->GetIsInCloud() || enemies[idx]->GetDistFromPlayer() <= std::powf(180.0f, 2.0f))
		{
			es->SetVisible(false);
			idx++;
			continue;
		}

		dx::XMFLOAT3 vec = enemies[idx]->GetPosition() - game->GetPlayer()->GetPosition();
		dx::XMFLOAT3 relativePos = vec / (range / minimapRadius);
		dx::XMFLOAT2 pos = {
			cos(angle)*relativePos.x - sin(angle)* -relativePos.z,
			sin(angle)*relativePos.x + cos(angle)* -relativePos.z
		};

		if ((pos.x >= -minimapRadius && pos.x <= minimapRadius) &&
			(pos.y >= -minimapRadius && pos.y <= minimapRadius))
		{
			es->SetVisible(true);
			es->GetOwner()->SetPosition(dx::XMFLOAT3{ pos.x + mOrigin.x, pos.y + mOrigin.y, 0.0f });
			es->GetOwner()->SetRotation(dx::XMFLOAT3{ 0.0f, 0.0f, enemies[idx]->GetRotation().y + angle });
		}
		else
		{
			es->SetVisible(false);
		}

		idx++;
	}

	// direction sprites
	float dist = 200.0f;
	for (int i = 0; i < 4; i++)
	{
		Actor* sprite = mDirectionSprites[i]->GetOwner();
		float sign = i % 2 == 0 ? 1.0f : -1.0f;

		if (i / 2 == 0)
		{
			sprite->SetPosition({
				-sin(angle)* sign*dist + mOrigin.x,
				+cos(angle)* sign*dist + mOrigin.y,
				0.0f
			});
		}
		else
		{
			sprite->SetPosition({
				+cos(-angle)* sign*dist + mOrigin.x,
				-sin(-angle)* sign*dist + mOrigin.y,
				0.0f
			});
		}
	}
}

void Minimap::RemoveEnemySprites()
{
	delete mEnemySprites.back();
	mEnemySprites.pop_back();
}

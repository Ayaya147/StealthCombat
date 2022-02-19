#include "EnemyActor.h"
#include "PlayerActor.h"
#include "GameScene.h"
#include "MeshComponent.h"
#include "MoveComponent.h"
#include "Mesh.h"
#include "Renderer.h"
#include "SphereComponent.h"
#include "Collision.h"
#include "DefineConstant.h"
#include "Random.h"
#include "XMFloatHelper.h"
#include "Minimap.h"
#include "PhysWorld.h"

namespace dx = DirectX;

EnemyActor::EnemyActor(BaseScene* scene)
	:
	Actor(scene),
	mDist(0.0f),
	mIsLockedOn(false),
	mIsInCloud(false)
{
	auto game = dynamic_cast<GameScene*>(GetScene());
	game->AddEnemy(this);

	float range = Constant::createRange;
	SetScale(0.1f);
	SetPosition(dx::XMFLOAT3{ Random::GetFloatRange(-range,range),Constant::height,Random::GetFloatRange(-range,range) });
	SetRotation(dx::XMFLOAT3{0.0f,Random::GetFloatRange(-Constant::PI,Constant::PI),0.0f });

	Renderer* renderer = GetScene()->GetRenderer();
	Mesh* mesh = renderer->GetMesh("enemy");
	mesh->ParseMesh(renderer, "enemy", L"Phong");
	MeshComponent* mc = new MeshComponent(this, mesh);

	mMoveComponent = new MoveComponent(this);
	mMoveComponent->SetForwardSpeedMax(10.0f);
	mMoveComponent->SetForwardSpeed(5.0f);
	mMoveComponent->SetAcceleration(3.0f);

	float radius = 10.0f;
	mSphereComponent = new SphereComponent(this);
	Sphere* sphere = new Sphere(GetPosition(), radius * GetScale().x);
	mSphereComponent->SetSphere(sphere);
	sphere = new Sphere(GetPosition(), radius * GetScale().x);
	mSphereComponent->SetSphereLast(sphere);
}

EnemyActor::~EnemyActor()
{
	if (auto game = dynamic_cast<GameScene*>(GetScene()))
	{
		game->RemoveEnemy(this);
		game->GetMinimap()->RemoveEnemySprites(game, this);
	}
}

void EnemyActor::UpdateActor(float deltaTime)
{
	auto game = dynamic_cast<GameScene*>(GetScene());
	PhysWorld* phys = game->GetPhysWorld();
	if (phys->IsCollidedWithCloud(mSphereComponent))
	{
		mIsInCloud = true;
	}
	else
	{
		mIsInCloud = false;
	}

	float range = Constant::createRange;
	if (GetPosition().x < -range ||
		GetPosition().x > range ||
		GetPosition().z < -range ||
		GetPosition().z > range)
	{
		SetRotation(dx::XMFLOAT3{ 0.0f, GetRotation().y + Constant::PI, 0.0f });
	}
}

void EnemyActor::CalcDistFromPlayer()
{
	auto game = dynamic_cast<GameScene*>(GetScene());
	mDist = DXMath::LengthSq(game->GetPlayer()->GetPosition() - GetPosition());
}

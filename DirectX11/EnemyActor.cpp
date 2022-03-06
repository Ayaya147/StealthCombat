#include "EnemyActor.h"
#include "PlayerActor.h"
#include "MissileActor.h"
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
	mIsLockedOn(false),
	mIsInCloud(false),
	mTime(Random::GetFloatRange(6.0f, 12.0f))
{
	auto game = dynamic_cast<GameScene*>(GetScene());
	game->AddEnemy(this);

	SetScale(0.12f);
	SetRotation(dx::XMFLOAT3{ 0.0f,Random::GetFloatRange(-Constant::PI,Constant::PI),0.0f });

	float range = Constant::createRange * 0.9f;
	dx::XMFLOAT3 pos = { Random::GetFloatRange(-range,range),Constant::height,Random::GetFloatRange(-range,range) };
	dx::XMFLOAT3 player = { 0.0f,Constant::height,0.0f};
	while (DXMath::LengthSq(pos - player) < std::powf(20.0f, 2.0f))
	{
		pos = { Random::GetFloatRange(-range,range),Constant::height,Random::GetFloatRange(-range,range) };
	}
	SetPosition(pos);
	
	CalcNextPos();

	Renderer* renderer = GetScene()->GetRenderer();
	Mesh* mesh = renderer->GetMesh("enemy");
	mesh->ParseMesh(renderer, "enemy", L"Phong");
	MeshComponent* mc = new MeshComponent(this, mesh);

	mMoveComponent = new MoveComponent(this);
	mMoveComponent->SetForwardSpeedMax(10.0f);
	mMoveComponent->SetForwardSpeed(7.0f);
	mMoveComponent->SetAcceleration(1.0f);

	float radius = 10.0f;
	mBody = new SphereComponent(this);
	Sphere* sphere = new Sphere(GetPosition(), radius * GetScale().x);
	mBody->SetSphere(sphere);
	sphere = new Sphere(GetPosition(), radius * GetScale().x);
	mBody->SetSphereLast(sphere);

	mAttackRange = new SphereComponent(this);
	sphere = new Sphere(GetPosition(), 12.0f);
	mAttackRange->SetSphere(sphere);
	sphere = new Sphere(GetPosition(), 12.0f);
	mAttackRange->SetSphereLast(sphere);

	mSign = Random::GetIntRange(0, 1) == 0 ? -1.0f : 1.0f;
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
	if (phys->IsCollidedWithCloud(mBody))
	{
		mIsInCloud = true;
	}
	else
	{
		mIsInCloud = false;
	}

	PlayerActor* player = game->GetPlayer();
	if (phys->IsCollidedWithPlayer(mAttackRange) &&
		DXMath::Dot(player->GetPosition() - GetPosition(), GetForward()) > 0.0f &&
		!player->GetIsLockedOn() &&
		!player->GetIsInCloud())
	{
		player->SetLockedOn(true);
		MissileActor* missile = new MissileActor(
			GetScene(), player, GetPosition(), mMoveComponent->GetForwardSpeed()
		);
	}

	if (CalcAngle() > 0.2f)
	{
		mMoveComponent->SetAngularSpeed(mSign * 0.6f);
		mMoveComponent->SetMoveType(MoveComponent::MoveType::ECornering);
	}
	else
	{
		mMoveComponent->SetMoveType(MoveComponent::MoveType::EStraight);
		mSign = Random::GetIntRange(0, 1) == 0 ? -1.0f : 1.0f;
		mTime -= deltaTime;
	}

	if (mTime <= 0.0f)
	{
		CalcNextPos();
		mTime = Random::GetFloatRange(6.0f, 12.0f);
	}
}

void EnemyActor::CalcNextPos()
{
	float range = Constant::createRange * 0.9f;
	dx::XMFLOAT3 pos = { Random::GetFloatRange(-range,range),Constant::height,Random::GetFloatRange(-range,range) };
	while (DXMath::LengthSq(pos - GetPosition()) < std::powf(150.0f, 2.0f))
	{
		pos = { Random::GetFloatRange(-range,range),Constant::height,Random::GetFloatRange(-range,range) };
	}

	mTargetLocationPos = pos;
}

float EnemyActor::CalcAngle()
{
	dx::XMFLOAT3 vec1 = mTargetLocationPos - GetPosition();
	dx::XMFLOAT3 vec2 = GetForward();
	float cos = DXMath::Dot(vec1, vec2) / (DXMath::Length(vec1) * DXMath::Length(vec2));

	return std::acosf(cos);
}

#include "MissileActor.h"
#include "EnemyActor.h"
#include "GameScene.h"
#include "MeshComponent.h"
#include "MoveComponent.h"
#include "Mesh.h"
#include "Renderer.h"
#include "SphereComponent.h"
#include "Collision.h"
#include "DefineConstant.h"
#include "XMFloatHelper.h"
#include "PhysWorld.h"
#include "Collision.h"

namespace dx = DirectX;

MissileActor::MissileActor(BaseScene* scene, Actor* actor, const DirectX::XMFLOAT3& pos, float forwardSpeed)
	:
	Actor(scene),
	mTarget(actor)
{
	if (auto enemy = dynamic_cast<EnemyActor*>(actor))
	{
		mType = MissileType::ETargetEnemy;
	}
	else
	{
		mType = MissileType::ETargetPlayer;
	}

	Renderer* renderer = GetScene()->GetRenderer();
	switch (mType)
	{
	case MissileActor::MissileType::ETargetEnemy:
	{
		Mesh* mesh = renderer->GetMesh("missile");
		mesh->ParseMesh(renderer, "missile", L"Phong");
		MeshComponent* mc = new MeshComponent(this, mesh);
	}
		break;

	case MissileActor::MissileType::ETargetPlayer:
	{
		Mesh* mesh = renderer->GetMesh("missile1");
		mesh->ParseMesh(renderer, "missile1", L"Phong");
		MeshComponent* mc = new MeshComponent(this, mesh);
	}
		break;
	}

	SetScale(0.08f);
	SetPosition(pos);
	dx::XMFLOAT3 forward = mTarget->GetPosition() - GetPosition();
	SetRotation(dx::XMFLOAT3{ 0.0f,atan2f(forward.x, forward.z),0.0f });

	mMoveComponent = new MoveComponent(this);
	mMoveComponent->SetForwardSpeedMax(25.0f);
	mMoveComponent->SetForwardSpeed(forwardSpeed);
	mMoveComponent->SetAcceleration(8.0f);

	float radius = 8.0f;
	mSphereComponent = new SphereComponent(this);
	Sphere* sphere = new Sphere(GetPosition(), radius * GetScale().x);
	mSphereComponent->SetSphere(sphere);
	sphere = new Sphere(GetPosition(), radius * GetScale().x);
	mSphereComponent->SetSphereLast(sphere);
}

void MissileActor::UpdateActor(float deltaTime)
{
	dx::XMFLOAT3 forward = mTarget->GetPosition() - GetPosition();
	SetRotation(dx::XMFLOAT3{ 0.0f,atan2f(forward.x, forward.z),0.0f });

	auto game = dynamic_cast<GameScene*>(GetScene());
	PhysWorld* phys = game->GetPhysWorld();
	PhysWorld::CollisionInfo info;

	switch (mType)
	{
	case MissileActor::MissileType::ETargetEnemy:
	{
		auto enemy = dynamic_cast<EnemyActor*>(mTarget);
		if (phys->IsCollidedWithEnemy(mSphereComponent, enemy->GetSphereComp()))
		{
			enemy->SetActorState(Actor::ActorState::EDead);
			SetActorState(Actor::ActorState::EDead);
		}
	}
		break;

	case MissileActor::MissileType::ETargetPlayer:
		if (phys->IsCollidedWithPlayer(mSphereComponent))
		{
			game->SetSceneState(BaseScene::SceneState::EQuit);
		}
		break;
	}
}

#include "PlayerActor.h"
#include "EnemyActor.h"
#include "MissileActor.h"
#include "ExplosionActor.h"
#include "GameScene.h"
#include "MeshComponent.h"
#include "MoveComponent.h"
#include "SphereComponent.h"
#include "CameraComponent.h"
#include  "SpriteComponent.h"
#include "Mesh.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "InputSystem.h"
#include "Keyboard.h"
#include "GamePad.h"
#include "Collision.h"
#include "PhysWorld.h"
#include "DefineConstant.h"
#include "XMFloatHelper.h"

namespace dx = DirectX;

static constexpr float accelW = 2.5f;
static constexpr float accelS = -3.5f;
static constexpr float accelNatural = -1.0f;
static constexpr float angularSpd = 0.8f;
static constexpr float angularRate1 = 1.6f;
static constexpr float angularRate2 = 1.2f;

PlayerActor::PlayerActor(BaseScene* scene)
	:
	Actor(scene),
	mOutCloudTime(0.0f),
	mTargetEnemy(nullptr)
{
	SetScale(0.125f);
	SetPosition(dx::XMFLOAT3{ 0.0f,Constant::height,0.0f });

	Renderer* renderer = GetScene()->GetRenderer();	
	Mesh* mesh = renderer->GetMesh("player");
	mesh->ParseMesh(renderer, "player", L"Phong");
	MeshComponent* mc = new MeshComponent(this, mesh);

	CameraComponent* cc = new CameraComponent(this);
	cc->SnapToIdeal();

	mMoveComponent = new MoveComponent(this);
	mMoveComponent->SetForwardSpeedMax(15.0f);

	if (auto game = dynamic_cast<GameScene*>(scene))
	{
		mMoveComponent->SetForwardSpeed(8.0f);
		mMoveComponent->SetForwardSpeedMin(600.0f / 160.0f);

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
	}
}

void PlayerActor::ActorInput()
{
	InputSystem* input = GetScene()->GetInputSystem();
	GamePad* pad = input->GetPad();

	if (input->GetPlayerRightTurn() && !input->GetPlayerLeftTurn())
	{
		mMoveComponent->SetMoveType(MoveComponent::MoveType::ECornering);
		pad->SetRightVibration(100);

		if (input->GetPlayerDecel() && !input->GetPlayerAccel())
		{
			mMoveComponent->SetAngularSpeed(angularSpd * angularRate1);
		}
		else if(input->GetPlayerAccel() && !input->GetPlayerDecel())
		{
			mMoveComponent->SetAngularSpeed(angularSpd);
		}
		else
		{
			mMoveComponent->SetAngularSpeed(angularSpd * angularRate2);
		}
	}
	else if (input->GetPlayerLeftTurn() && !input->GetPlayerRightTurn())
	{
		mMoveComponent->SetMoveType(MoveComponent::MoveType::ECornering);
		pad->SetLeftVibration(100);

		if (input->GetPlayerDecel() && !input->GetPlayerAccel())
		{
			mMoveComponent->SetAngularSpeed(-angularSpd * angularRate1);
		}
		else if (input->GetPlayerAccel() && !input->GetPlayerDecel())
		{
			mMoveComponent->SetAngularSpeed(-angularSpd);
		}
		else
		{
			mMoveComponent->SetAngularSpeed(-angularSpd * angularRate2);
		}
	}
	else
	{
		mMoveComponent->SetMoveType(MoveComponent::MoveType::EStraight);
		pad->StopVibration();
	}

	if (input->GetPlayerAccel() && !input->GetPlayerDecel())
	{
		mMoveComponent->SetAcceleration(accelW);
	}
	else if (input->GetPlayerDecel() && !input->GetPlayerAccel())
	{
		mMoveComponent->SetAcceleration(accelS);
	}
	else
	{
		mMoveComponent->SetAcceleration(accelNatural);
	}

	if (auto game = dynamic_cast<GameScene*>(GetScene()))
	{
		SpriteComponent* sprite = game->GetMarkingEnemySprite();
		if (input->GetPlayerEmitMissile() && sprite->GetIsVisible())
		{
			mTargetEnemy->SetLockedOn(true);
			MissileActor* missile = new MissileActor(
				GetScene(), mTargetEnemy, GetPosition(), mMoveComponent->GetForwardSpeed()
			);
		}
	}
}

void PlayerActor::UpdateActor(float deltaTime)
{
	if (auto game = dynamic_cast<GameScene*>(GetScene()))
	{
		PhysWorld* phys = game->GetPhysWorld();
		PhysWorld::CollisionInfo info;

		if (phys->IsCollidedWithCloud(mBody))
		{
			mOutCloudTime = 0.0f;
		}
		else
		{
			mOutCloudTime += deltaTime;
		}

		if (phys->IsCollidedWithEnemy(mBody, info))
		{
			ExplosionActor* explosion = new ExplosionActor(game);
			explosion->SetPosition(GetPosition() + GetForward() * 1.25f);
			game->SetSceneState(BaseScene::SceneState::EQuit);
		}

		SpriteComponent* sprite = game->GetMarkingPlayerSprite();
		if (mIsLockedOn)
		{
			sprite->SetVisible(true);
			dx::XMFLOAT3 lastPos = LocalToClip(this);
			sprite->GetOwner()->SetPosition(lastPos);
		}
		else
		{
			sprite->SetVisible(false);
		}

		sprite = game->GetMarkingEnemySprite();
		sprite->SetVisible(false);
		if (!game->GetEnemies().empty() &&
			phys->IsCollidedWithEnemy(mAttackRange, info))
		{
			mTargetEnemy = dynamic_cast<EnemyActor*>(info.mActor);
			if (!mTargetEnemy->GetIsLockedOn())
			{
				dx::XMFLOAT3 lastPos = LocalToClip(mTargetEnemy);

				if (lastPos.y > -540.0f &&
					lastPos.y < 0.0f &&
					lastPos.x > -960.0f &&
					lastPos.x < 960.0f)
				{
					sprite->SetVisible(true);
					sprite->GetOwner()->SetPosition(lastPos);
				}
			}
		}
	}
}

DirectX::XMFLOAT3 PlayerActor::LocalToClip(Actor* actor)
{
	Renderer* renderer = GetScene()->GetRenderer();

	dx::XMVECTOR localPos = dx::XMVectorZero();
	dx::XMMATRIX worldtransform = actor->GetWorldTransform();
	dx::XMMATRIX view = renderer->GetViewMatrix();
	dx::XMMATRIX projection = renderer->GetProjectionMatrix();
	dx::XMMATRIX matrix = worldtransform * view * projection;

	dx::XMVECTOR ndc = dx::XMVector3TransformCoord(localPos, matrix);
	float ndcX = dx::XMVectorGetX(ndc);
	float ndcY = dx::XMVectorGetY(ndc);

	return dx::XMFLOAT3{ ndcX * 960.0f, -ndcY * 540.0f, 0.0f };
}

float PlayerActor::GetForwardSpeed() const
{
	return mMoveComponent->GetForwardSpeed();
}

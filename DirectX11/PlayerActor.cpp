#include "PlayerActor.h"
#include "EnemyActor.h"
#include "MissileActor.h"
#include "ExplosionActor.h"
#include "EmitterActor.h"
#include "GameScene.h"
#include "MeshComponent.h"
#include "MoveComponent.h"
#include "SphereComponent.h"
#include "CameraComponent.h"
#include "SpriteComponent.h"
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
#include "Texture.h"

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
	mOutCloudTime(13.99f),
	mEmitterCD(0.05f),
	mPlayerSprite(nullptr),
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

		Actor* sprite = new Actor(GetScene());
		Texture* tex = renderer->GetTexture("player");
		mPlayerSprite = new SpriteComponent(sprite, tex);
		sprite->SetScale(0.1f);
		mPlayerSprite->SetVisible(false);
	}
}

void PlayerActor::ActorInput()
{
	auto game = dynamic_cast<GameScene*>(GetScene());
	InputSystem* input = GetScene()->GetInputSystem();
	GamePad* pad = input->GetPad();
	pad->StopVibration();

	if (input->GetPlayerRightTurn() && !input->GetPlayerLeftTurn())
	{
		mMoveComponent->SetMoveType(MoveComponent::MoveType::ECornering);
		if (game)
		{
			pad->SetRightVibration(50);
		}

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
		if (game)
		{
			pad->SetLeftVibration(50);
		}

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

		if (game)
		{
			if (mOutCloudTime != 14.0f)
			{
				pad->StopVibration();
			}
			else
			{
				pad->SetVibration(3);
			}
		}
	}

	if (input->GetPlayerAccel() && !input->GetPlayerDecel())
	{
		mEmitterCD -= GetScene()->GetDeltaTime();
		mMoveComponent->SetAcceleration(accelW);
	}
	else if (input->GetPlayerDecel() && !input->GetPlayerAccel())
	{
		if (game)
		{
			pad->SetVibration(100);
		}
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
			if (mOutCloudTime > 0.0f)
			{
				mOutCloudTime = 14.0f;
			}
		}
		else
		{
			mOutCloudTime -= deltaTime;
			if (mOutCloudTime <= 0.0f)
			{
				mOutCloudTime = 0.0f;
			}
		}

		if (phys->IsCollidedWithEnemy(mBody, info))
		{
			ExplosionActor* explosion = new ExplosionActor(game);
			explosion->SetPosition(GetPosition() + GetForward() * 1.25f);
			game->SetSceneState(BaseScene::SceneState::EGameEnd);
		}

		SpriteComponent* sprite = game->GetMarkingPlayerSprite();
		if (mIsLockedOn)
		{
			sprite->SetVisible(true);
			dx::XMFLOAT3 clipPos = LocalToClip(this);
			sprite->GetOwner()->SetPosition(clipPos);
		}
		else
		{
			sprite->SetVisible(false);
		}

		sprite = game->GetMarkingEnemySprite();
		sprite->SetVisible(false);
		if (!game->GetEnemies().empty() && phys->IsCollidedWithEnemy(mAttackRange, info))
		{
			mTargetEnemy = dynamic_cast<EnemyActor*>(info.mActor);
			if (!mTargetEnemy->GetIsLockedOn())
			{
				dx::XMFLOAT3 clipPos = LocalToClip(mTargetEnemy);

				if (clipPos.y > -540.0f &&
					clipPos.y < 0.0f &&
					clipPos.x > -960.0f &&
					clipPos.x < 960.0f)
				{
					sprite->SetVisible(true);
					sprite->GetOwner()->SetPosition(clipPos);
				}
			}
		}

		if (mEmitterCD <= 0.0f)
		{
			EmitterActor* emitter = new EmitterActor(GetScene());
			emitter->SetPosition(GetPosition() - GetForward() * 1.3f);
			emitter->SetRotation(GetRotation());
			emitter->SetScale(1.5f);
			mEmitterCD = 0.05f;
		}

		if (mOutCloudTime == 14.0f && !mIsLockedOn)
		{
			Actor* actor = mPlayerSprite->GetOwner();
			mPlayerSprite->SetVisible(true);
			dx::XMFLOAT3 clipPos = LocalToClip(this);
			actor->SetPosition(clipPos);
		}
		else
		{
			mPlayerSprite->SetVisible(false);
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

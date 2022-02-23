#include "PlayerActor.h"
#include "EnemyActor.h"
#include "MissileActor.h"
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

static constexpr float accelW = 3.0f;
static constexpr float accelS = -4.0f;
static constexpr float accelNatural = -1.0f;
static constexpr float angularSpd = 0.8f;
static constexpr float angularRate1 = 0.92f;
static constexpr float angularRate2 = 1.7f;

PlayerActor::PlayerActor(BaseScene* scene)
	:
	Actor(scene),
	mOutCloudTime(0.0f),
	mTargetEnemy(nullptr)
{
	SetScale(0.1f);
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

	if (input->GetPlayerRightTurn())
	{
		pad->SetRightVibration(100);
		if (input->GetPlayerDecel() && !input->GetPlayerAccel())
		{
			mMoveComponent->SetAngularSpeed(angularSpd * angularRate2);
		}
		else
		{
			mMoveComponent->SetAngularSpeed(angularSpd);
		}
	}
	else if (input->GetPlayerLeftTurn())
	{
		pad->SetRightVibration(100);
		if (input->GetPlayerDecel() && !input->GetPlayerAccel())
		{
			mMoveComponent->SetAngularSpeed(-angularSpd * angularRate2);
		}
		else
		{
			mMoveComponent->SetAngularSpeed(-angularSpd);
		}
	}
	else
	{
		pad->StopVibration();
		float spd = mMoveComponent->GetAngularSpeed();
		mMoveComponent->SetAngularSpeed(spd * angularRate1);
		DirectX::XMFLOAT3 rotation = GetRotation();
		rotation.z = GetRotation().z * angularRate1;
		SetRotation(rotation);
	}

	if (input->GetPlayerAccel())
	{
		mMoveComponent->SetAcceleration(accelW);
	}
	else if (input->GetPlayerDecel())
	{
		mMoveComponent->SetAcceleration(accelS);
	}
	else
	{
		mMoveComponent->SetAcceleration(accelNatural);
	}

	if (auto game = dynamic_cast<GameScene*>(GetScene()))
	{
		SpriteComponent* sprite = game->GetMarkingSprite();
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
			game->SetSceneState(BaseScene::SceneState::EQuit);
		}

		SpriteComponent* sprite = game->GetMarkingSprite();
		sprite->SetVisible(false);
		if (!game->GetEnemies().empty() &&
			phys->IsCollidedWithEnemy(mAttackRange, info))
		{
			mTargetEnemy = dynamic_cast<EnemyActor*>(info.mActor);
			if (!mTargetEnemy->GetIsLockedOn())
			{
				Renderer* renderer = game->GetRenderer();

				dx::XMVECTOR localPos = dx::XMVectorZero();
				dx::XMMATRIX worldtransform = mTargetEnemy->GetWorldTransform();
				dx::XMMATRIX view = renderer->GetViewMatrix();
				dx::XMMATRIX projection = renderer->GetProjectionMatrix();
				dx::XMMATRIX matrix = worldtransform * view * projection;

				dx::XMVECTOR ndc = dx::XMVector3TransformCoord(localPos, matrix);
				float ndcX = dx::XMVectorGetX(ndc);
				float ndcY = dx::XMVectorGetY(ndc);
				dx::XMFLOAT3 lastPos = { ndcX * 960.0f, -ndcY * 540.0f, 0.0f };

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

	dx::XMFLOAT3 rotation = GetRotation();
	if (rotation.z > 0.8f)
	{
		rotation.z = 0.8f;
	}
	else if (rotation.z < -0.8f)
	{
		rotation.z = -0.8f;
	}
	SetRotation(rotation);
}

float PlayerActor::GetForwardSpeed() const
{
	return mMoveComponent->GetForwardSpeed();
}

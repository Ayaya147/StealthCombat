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
static constexpr float angularSpd = 1.0f;
static constexpr float angularRate1 = 0.92f;
static constexpr float angularRate2 = 1.5f;

PlayerActor::PlayerActor(BaseScene* scene)
	:
	Actor(scene),
	mOutCloudTime(0.0f)
{
	SetScale(0.1f);
	SetPosition(dx::XMFLOAT3{ 0.0f,Constant::height,0.0f });

	Renderer* renderer = GetScene()->GetRenderer();	
	Mesh* mesh = renderer->GetMesh("player");
	mesh->ParseMesh(renderer, "player", L"Phong");
	MeshComponent* mc = new MeshComponent(this, mesh);

	mMoveComponent = new MoveComponent(this);
	mMoveComponent->SetForwardSpeedMax(15.0f);
	mMoveComponent->SetForwardSpeed(6.0f);

	float radius = 10.0f;
	mBody = new SphereComponent(this);
	Sphere* sphere = new Sphere(GetPosition(), radius * GetScale().x);
	mBody->SetSphere(sphere);
	sphere = new Sphere(GetPosition(), radius * GetScale().x);
	mBody->SetSphereLast(sphere);

	mAttackRange = new SphereComponent(this);
	sphere = new Sphere(GetPosition(), 12.0f);
	mAttackRange->SetSphere(sphere);

	CameraComponent* cc = new CameraComponent(this);
	cc->SnapToIdeal();
}

void PlayerActor::UpdateActor(float deltaTime)
{
	auto game = dynamic_cast<GameScene*>(GetScene());
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

	if (phys->IsCollidedWithMissile(mBody, info))
	{
		//info.mActor->SetActorState(Actor::ActorState::EDead);
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

void PlayerActor::ActorInput()
{
	auto game = dynamic_cast<GameScene*>(GetScene());
	Keyboard* keyboard = game->GetInputSystem()->GetKeyboard();
	GamePad* pad = game->GetInputSystem()->GetPad();

	//GamePad
	if (pad->GetIsGamePad())
	{
		if (pad->GetThumbLeftX() > 0)
		{
			pad->SetRightVibration(100);
			if (pad->GetLeftTrigger() && !pad->GetRightTrigger())
			{
				mMoveComponent->SetAngularSpeed(angularSpd * angularRate2);
			}
			else
			{
				mMoveComponent->SetAngularSpeed(angularSpd);
			}
		}
		else if (pad->GetThumbLeftX() < 0)
		{
			pad->SetRightVibration(100);
			if (pad->GetLeftTrigger() && !pad->GetRightTrigger())
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

		if (pad->GetRightTrigger())
		{
			mMoveComponent->SetAcceleration(accelW);
		}
		else if (pad->GetLeftTrigger())
		{
			mMoveComponent->SetAcceleration(accelS);
		}
		else
		{
			mMoveComponent->SetAcceleration(accelNatural);
		}

		SpriteComponent* sprite = game->GetMarkingSprite();
		PhysWorld* phys = game->GetPhysWorld();
		PhysWorld::CollisionInfo info;
		if (!game->GetEnemies().empty() &&
			phys->IsAttackRangeCollidedWithEnemy(mAttackRange, info))
		{
			EnemyActor* enemy = dynamic_cast<EnemyActor*>(info.mActor);
			if (!enemy->GetIsLockedOn())
			{
				Renderer* renderer = game->GetRenderer();

				dx::XMVECTOR localPos = dx::XMVectorZero();
				dx::XMMATRIX worldtransform = enemy->GetWorldTransform();
				dx::XMMATRIX view = renderer->GetViewMatrix();
				dx::XMMATRIX projection = renderer->GetProjectionMatrix();
				dx::XMMATRIX matrix = worldtransform * view * projection;

				dx::XMVECTOR ndc = dx::XMVector3TransformCoord(localPos, matrix);
				float ndcX = dx::XMVectorGetX(ndc);
				float ndcY = dx::XMVectorGetY(ndc);
				dx::XMFLOAT3 lastPos = { ndcX * 960.0f, -ndcY * 540.0f, 0.0f };
				sprite->GetOwner()->SetPosition(lastPos);

				if (lastPos.y > -540.0f &&
					lastPos.y < 0.0f &&
					lastPos.x > -960.0f &&
					lastPos.x < 960.0f)
				{
					sprite->SetVisible(true);
					if (pad->GetButtonState(XINPUT_GAMEPAD_Y) == ButtonState::EPressed)
					{
						enemy->SetLockedOn(true);
						MissileActor* missile = new MissileActor(
							GetScene(), enemy, GetPosition(), mMoveComponent->GetForwardSpeed()
						);
					}
				}
				else
				{
					sprite->SetVisible(false);
				}
			}
			else
			{
				sprite->SetVisible(false);
			}
		}
		else
		{
			sprite->SetVisible(false);
		}
	}
	//Keyboard
	else
	{
		if (keyboard->GetKeyValue('D'))
		{
			if (keyboard->GetKeyValue('S') && !keyboard->GetKeyValue('W'))
			{
				mMoveComponent->SetAngularSpeed(angularSpd * angularRate2);
			}
			else
			{
				mMoveComponent->SetAngularSpeed(angularSpd);
			}
		}
		else if (keyboard->GetKeyValue('A'))
		{
			if (keyboard->GetKeyValue('S') && !keyboard->GetKeyValue('W'))
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
			float spd = mMoveComponent->GetAngularSpeed();
			mMoveComponent->SetAngularSpeed(spd * angularRate1);
			DirectX::XMFLOAT3 rotation = GetRotation();
			rotation.z = GetRotation().z * angularRate1;
			SetRotation(rotation);
		}

		if (keyboard->GetKeyValue('W'))
		{
			mMoveComponent->SetAcceleration(accelW);
		}
		else if (keyboard->GetKeyValue('S'))
		{
			mMoveComponent->SetAcceleration(accelS);
		}
		else
		{
			mMoveComponent->SetAcceleration(accelNatural);
		}

		SpriteComponent* sprite = game->GetMarkingSprite();
		PhysWorld* phys = game->GetPhysWorld();
		PhysWorld::CollisionInfo info;
		if (!game->GetEnemies().empty() &&
			phys->IsAttackRangeCollidedWithEnemy(mAttackRange, info))
		{
			EnemyActor* enemy = dynamic_cast<EnemyActor*>(info.mActor);
			if (!enemy->GetIsLockedOn())
			{
				Renderer* renderer = game->GetRenderer();

				dx::XMVECTOR localPos = dx::XMVectorZero();
				dx::XMMATRIX worldtransform = enemy->GetWorldTransform();
				dx::XMMATRIX view = renderer->GetViewMatrix();
				dx::XMMATRIX projection = renderer->GetProjectionMatrix();
				dx::XMMATRIX matrix = worldtransform * view * projection;

				dx::XMVECTOR ndc = dx::XMVector3TransformCoord(localPos, matrix);
				float ndcX = dx::XMVectorGetX(ndc);
				float ndcY = dx::XMVectorGetY(ndc);
				dx::XMFLOAT3 lastPos = { ndcX * 960.0f, -ndcY * 540.0f, 0.0f };
				sprite->GetOwner()->SetPosition(lastPos);

				if (lastPos.y > -540.0f &&
					lastPos.y < 0.0f &&
					lastPos.x > -960.0f &&
					lastPos.x < 960.0f)
				{
					sprite->SetVisible(true);
					if (keyboard->GetKeyState(VK_SPACE) == ButtonState::EPressed)
					{
						enemy->SetLockedOn(true);
						MissileActor* missile = new MissileActor(
							GetScene(), enemy, GetPosition(), mMoveComponent->GetForwardSpeed()
						);
					}
				}
				else
				{
					sprite->SetVisible(false);
				}
			}
			else
			{
				sprite->SetVisible(false);
			}
		}
		else
		{
			sprite->SetVisible(false);
		}
	}
}

float PlayerActor::GetForwardSpeed() const
{
	return mMoveComponent->GetForwardSpeed();
}

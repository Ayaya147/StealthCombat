#include "PlayerActor.h"
#include "NumberActor.h"
#include "EnemyActor.h"
#include "MissileActor.h"
#include "GameScene.h"
#include "MeshComponent.h"
#include "MoveComponent.h"
#include "SphereComponent.h"
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
	mMoveComponent->SetForwardSpeed(1.0f);

	float radius = 10.0f;
	mSphereComponent = new SphereComponent(this);
	Sphere* sphere = new Sphere(GetPosition(), radius * GetScale().x);
	mSphereComponent->SetSphere(sphere);
	sphere = new Sphere(GetPosition(), radius * GetScale().x);
	mSphereComponent->SetSphereLast(sphere);

	mCloudTimeNum = new NumberActor(GetScene(), 0, 4);
	mCloudTimeNum->SetOriPosition(dx::XMFLOAT3{ 242.0f, -6.0f, 0.0f });
	mCloudTimeNum->SetScale(0.6f);

	mSpdNum = new NumberActor(GetScene(), 0, 4);
	mSpdNum->SetOriPosition(dx::XMFLOAT3{ -178.0f, -6.0f, 0.0f });
	mSpdNum->SetScale(0.6f);
}

void PlayerActor::UpdateActor(float deltaTime)
{
	auto game = dynamic_cast<GameScene*>(GetScene());
	PhysWorld* phys = game->GetPhysWorld();
	PhysWorld::CollisionInfo info;

	if (phys->IsCollidedWithCloud(mSphereComponent))
	{
		mOutCloudTime = 0.0f;
	}
	else
	{
		mOutCloudTime += deltaTime;
	}
	mCloudTimeNum->SetValue(mOutCloudTime * 100.0f);
	mSpdNum->SetValue(GetForwardSpeed() * 160.0f);

	if (phys->IsCollidedWithEnemy(mSphereComponent, info))
	{
		//info.mActor->SetActorState(Actor::ActorState::EDead);
	}

	//if (phys->IsCollidedWithMissile(mSphereComponent, info))
	//{
	//	info.mActor->SetActorState(Actor::ActorState::EDead);
	//}

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
	Keyboard* keyboard = GetScene()->GetInputSystem()->GetKeyboard();
	GamePad* pad = GetScene()->GetInputSystem()->GetPad();
	auto game = dynamic_cast<GameScene*>(GetScene());

	DirectX::XMFLOAT3 rotation = GetRotation();
	float accelW = 3.0f;
	float accelS = -5.5f;
	float accelNatural = -1.5f;
	float angularSpd = 1.3f;

	if (pad->GetIsGamePad())
	{
		if (pad->GetThumbLeftX() > 0)
		{
			pad->SetRightVibration(100);
			mMoveComponent->SetAngularSpeed(angularSpd);
		}
		else if (pad->GetThumbLeftX() < 0)
		{
			pad->SetLeftVibration(100);
			mMoveComponent->SetAngularSpeed(-angularSpd);
		}
		else
		{
			pad->StopVibration();
			float spd = mMoveComponent->GetAngularSpeed();
			mMoveComponent->SetAngularSpeed(spd * 0.92f);
			rotation.z = GetRotation().z * 0.92f;
			SetRotation(rotation);
		}

		if (pad->GetRightTrigger())
		{
			mMoveComponent->SetAcceleration(accelW);
		}
		else if(pad->GetLeftTrigger())
		{
			mMoveComponent->SetAcceleration(accelS);
		}
		else
		{
			mMoveComponent->SetAcceleration(accelNatural);
		}
	}
	else
	{
		if (keyboard->GetKeyValue('D'))
		{
			mMoveComponent->SetAngularSpeed(angularSpd);
		}
		else if (keyboard->GetKeyValue('A'))
		{
			mMoveComponent->SetAngularSpeed(-angularSpd);
		}
		else
		{
			float spd = mMoveComponent->GetAngularSpeed();
			mMoveComponent->SetAngularSpeed(spd * 0.92f);
			rotation.z = GetRotation().z * 0.92f;
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

		if (keyboard->GetKeyState(VK_SPACE) == ButtonState::EPressed)
		{
			if (!game->GetEnemies().empty())
			{
				EnemyActor* enemy = game->GetEnemies()[0];
				float dist = enemy->GetDistFromPlayer();
				if (dist <= 100.0f && !enemy->GetIsLockedOn())
				{
					if (DXMath::Dot(enemy->GetPosition() - GetPosition(), GetForward()) > 0.0f)
					{
						enemy->SetLockedOn(true);
						MissileActor* missile = new MissileActor(GetScene(), enemy, GetPosition(), mMoveComponent->GetForwardSpeed());
					}
				}
			}
		}
	}
}

float PlayerActor::GetForwardSpeed() const
{
	return mMoveComponent->GetForwardSpeed();
}

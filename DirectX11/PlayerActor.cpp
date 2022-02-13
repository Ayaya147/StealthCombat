#include "PlayerActor.h"
#include "NumberActor.h"
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

namespace dx = DirectX;

PlayerActor::PlayerActor(BaseScene* scene)
	:
	Actor(scene),
	mOutCloudTime(0.0f)
{
	SetScale(0.1f);

	Renderer* renderer = GetScene()->GetRenderer();
	
	Mesh* mesh = renderer->GetMesh("player");
	mesh->ParseMesh(renderer, "player", L"Phong");
	MeshComponent* mc = new MeshComponent(this, mesh);

	mMoveComponent = new MoveComponent(this);
	mMoveComponent->SetForwardSpeedMax(15.0f);

	float radius = 10.0f;
	mSphereComponent = new SphereComponent(this);
	Sphere* sphere = new Sphere(GetPosition(), radius * GetScale().x);
	mSphereComponent->SetSphere(sphere);

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

	DirectX::XMFLOAT3 rotation = GetRotation();

	if (pad->GetIsGamePad())
	{
		if (pad->GetThumbLeftX() > 0)
		{
			pad->SetRightVibration(100);
			mMoveComponent->SetAngularSpeed(1.2f);
		}
		else if (pad->GetThumbLeftX() < 0)
		{
			pad->SetLeftVibration(100);
			mMoveComponent->SetAngularSpeed(-1.2f);
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
			mMoveComponent->SetAcceleration(3.0f);
		}
		else if(pad->GetLeftTrigger())
		{
			mMoveComponent->SetAcceleration(-5.0f);
		}
		else
		{
			mMoveComponent->SetAcceleration(-1.5f);
		}
	}
	else
	{
		if (keyboard->GetKeyValue('D'))
		{
			mMoveComponent->SetAngularSpeed(1.2f);
		}
		else if (keyboard->GetKeyValue('A'))
		{
			mMoveComponent->SetAngularSpeed(-1.2f);
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
			mMoveComponent->SetAcceleration(3.0f);
		}
		else if (keyboard->GetKeyValue('S'))
		{
			mMoveComponent->SetAcceleration(-5.0f);
		}
		else
		{
			mMoveComponent->SetAcceleration(-1.5f);
		}
	}
}

float PlayerActor::GetForwardSpeed() const
{
	return mMoveComponent->GetForwardSpeed();
}

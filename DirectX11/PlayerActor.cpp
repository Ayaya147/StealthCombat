#include "PlayerActor.h"
#include "EnemyActor.h"
#include "MissileActor.h"
#include "ExplosionActor.h"
#include "EmissionActor.h"
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
#include "AudioSystem.h"
#include "Keyboard.h"
#include "GamePad.h"
#include "Collision.h"
#include "PhysWorld.h"
#include "DefineConstant.h"
#include "XMFloatHelper.h"
#include "Texture.h"
#include "Random.h"
#include "TranslucenceComponent.h"

namespace dx = DirectX;

#define ACCEL_W 2.5f
#define ACCEL_S -3.5f
#define ACCEL_NATURAL -1.0f
#define ANGULAR_SPD 0.8f
#define ANGULAR_SPD1 1.3f
#define ANGULAR_SPD2 1.0f
#define OUT_CLOUD_TIME_MAX 14.0f

PlayerActor::PlayerActor(BaseScene* scene)
	:
	Actor(scene),
	mOutCloudTime(14.0f),
	mEmitterCD(0.05f),
	mPlayerTranslucenceComp(nullptr),
	mTargetEnemy(nullptr),
	mIsInCloud(false),
	mIsLockedOn(false),
	mIsLockOnSE(false),
	mAttackRange(nullptr),
	mBody(nullptr)
{
	SetScale(0.18f);
	SetPosition(dx::XMFLOAT3{ 0.0f,Constant::height,0.0f });

	Renderer* renderer = GetScene()->GetRenderer();	
	Mesh* mesh = renderer->GetMesh("player1");
	mesh->ParseMesh(renderer, "player1", L"Toon");
	MeshComponent* mc = new MeshComponent(this, mesh);

	mMoveComponent = new MoveComponent(this);
	mMoveComponent->SetForwardSpeedMax(15.0f);

	if (auto game = dynamic_cast<GameScene*>(scene))
	{
		mMoveComponent->SetForwardSpeed(8.0f);
		mMoveComponent->SetForwardSpeedMin(600.0f / 160.0f);

		float radius = 8.0f;
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

		Actor* actor = new Actor(GetScene());
		Mesh* mesh = renderer->GetMesh("planePlayer");
		mesh->ParsePlaneMesh(renderer, "player_guide", L"Phong", 2, 1.4f, 1.4f, true);
		mPlayerTranslucenceComp = new TranslucenceComponent(actor, mesh, 300);

		mCameraComponent = new CameraComponent(this, CameraComponent::CameraType::ESpring);
	}
	else
	{
		mCameraComponent = new CameraComponent(this, CameraComponent::CameraType::ENormal);
	}

	mCameraComponent->SnapToIdeal();
}

void PlayerActor::ActorInput()
{
	auto game = dynamic_cast<GameScene*>(GetScene());
	InputSystem* input = GetScene()->GetInputSystem();

	if (input->GetPlayerRightTurn() && !input->GetPlayerLeftTurn())
	{
		mMoveComponent->SetMoveType(MoveComponent::MoveType::ECornering);
		if (input->GetPlayerDecel() && !input->GetPlayerAccel())
		{
			mMoveComponent->SetAngularSpeed(ANGULAR_SPD1);
		}
		else if(input->GetPlayerAccel() && !input->GetPlayerDecel())
		{
			mMoveComponent->SetAngularSpeed(ANGULAR_SPD);
		}
		else
		{
			mMoveComponent->SetAngularSpeed(ANGULAR_SPD2);
		}
	}
	else if (input->GetPlayerLeftTurn() && !input->GetPlayerRightTurn())
	{
		mMoveComponent->SetMoveType(MoveComponent::MoveType::ECornering);
		if (input->GetPlayerDecel() && !input->GetPlayerAccel())
		{
			mMoveComponent->SetAngularSpeed(-ANGULAR_SPD1);
		}
		else if (input->GetPlayerAccel() && !input->GetPlayerDecel())
		{
			mMoveComponent->SetAngularSpeed(-ANGULAR_SPD);
		}
		else
		{
			mMoveComponent->SetAngularSpeed(-ANGULAR_SPD2);
		}
	}
	else
	{
		mMoveComponent->SetMoveType(MoveComponent::MoveType::EStraight);
	}

	if (input->GetPlayerAccel() && !input->GetPlayerDecel())
	{
		if (game)
		{
			if (mMoveComponent->GetForwardSpeed() < 15.0f)
			{
				game->SetVibrationStrength(100);
				if (mCameraComponent->GetCameraState() != CameraComponent::VibrationState::EHard)
				{
					mCameraComponent->SetCameraState(CameraComponent::VibrationState::ENormal);
				}
			}
			else
			{
				game->SetVibrationStrength(4);
				if (mCameraComponent->GetCameraState() != CameraComponent::VibrationState::EHard)
				{
					mCameraComponent->SetCameraState(CameraComponent::VibrationState::ELight);
				}
			}

			mEmitterCD -= GetScene()->GetDeltaTime();			
			game->GetAudioSystem()->SetVolume(game->GetAirplaneBGM(), 1.0f);
		}

		mMoveComponent->SetAcceleration(ACCEL_W);
	}
	else if (input->GetPlayerDecel() && !input->GetPlayerAccel())
	{
		if (game)
		{
			game->GetAudioSystem()->SetVolume(game->GetAirplaneBGM(), 0.5f);

			if (mMoveComponent->GetForwardSpeed() > 600.0f / 160.0f)
			{
				game->SetVibrationStrength(100);
				if (mCameraComponent->GetCameraState() != CameraComponent::VibrationState::EHard)
				{
					mCameraComponent->SetCameraState(CameraComponent::VibrationState::ENormal);
				}
			}
			else
			{
				game->SetVibrationStrength(0);
				if (mCameraComponent->GetCameraState() != CameraComponent::VibrationState::EHard)
				{
					mCameraComponent->SetCameraState(CameraComponent::VibrationState::ENone);
				}
			}
		}

		mMoveComponent->SetAcceleration(ACCEL_S);
	}
	else
	{
		if (game)
		{
			game->GetAudioSystem()->SetVolume(game->GetAirplaneBGM(), 0.5f);
			game->SetVibrationStrength(0);
			if (mCameraComponent->GetCameraState() != CameraComponent::VibrationState::EHard)
			{
				mCameraComponent->SetCameraState(CameraComponent::VibrationState::ENone);
			}
		}

		mMoveComponent->SetAcceleration(ACCEL_NATURAL);
	}

	if (game)
	{
		SpriteComponent* sprite = game->GetMarkingEnemySprite();
		if (input->GetPlayerEmitMissile() && sprite->GetIsVisible())
		{
			mTargetEnemy->SetLockedOn(true);
			MissileActor* missile = new MissileActor(
				GetScene(), mTargetEnemy, GetPosition(), mMoveComponent->GetForwardSpeed()
			);

			int index = game->GetAudioSystem()->LoadSound("se_missile");
			game->GetAudioSystem()->PlaySoundEx(index, 0);
		}
	}
}

void PlayerActor::UpdateActor(float deltaTime)
{
	if (auto game = dynamic_cast<GameScene*>(GetScene()))
	{
		PhysWorld* phys = game->GetPhysWorld();
		PhysWorld::CollisionInfo info;
		Renderer* renderer = game->GetRenderer();

		if (phys->IsCollidedWithCloud(mBody))
		{
			mIsInCloud = true;
			if (mOutCloudTime >= OUT_CLOUD_TIME_MAX)
			{
				mOutCloudTime = OUT_CLOUD_TIME_MAX;
			}
			else if (mOutCloudTime > 0.0f)
			{
				mOutCloudTime += deltaTime * 2.0f;
			}
		}
		else
		{
			mIsInCloud = false;
			mOutCloudTime -= deltaTime;
			if (mOutCloudTime <= 0.0f)
			{
				mOutCloudTime = 0.0f;
			}
		}

		if (phys->IsCollidedWithEnemy(mBody, info) && !mIsInCloud)
		{
			ExplosionActor* explosion = new ExplosionActor(game);
			explosion->SetScale(18.0f);
			explosion->SetPosition(GetPosition() + GetForward() * 1.25f);
			game->SetSceneState(BaseScene::SceneState::EGameEnd);

			int index = game->GetAudioSystem()->LoadSound("se_explosion");
			game->GetAudioSystem()->PlaySoundEx(index, 0);
		}

		SpriteComponent* sprite = game->GetMarkingPlayerSprite();
		if (mIsLockedOn)
		{
			sprite->SetVisible(true);
			dx::XMFLOAT3 clipPos = DXMath::LocalToClip(
				this->GetWorldTransform(),
				renderer->GetViewMatrix(),
				renderer->GetProjectionMatrix()
			);
			sprite->GetOwner()->SetPosition(clipPos);
		}
		else
		{
			sprite->SetVisible(false);
		}

		bool stopLockOnSE = true;
		int index = game->GetAudioSystem()->LoadSound("se_lockOn");
		sprite = game->GetMarkingEnemySprite();
		sprite->SetVisible(false);
		if (!game->GetEnemies().empty() && phys->IsCollidedWithEnemy(mAttackRange, info))
		{
			mTargetEnemy = dynamic_cast<EnemyActor*>(info.mActor);
			if (!mTargetEnemy->GetIsLockedOn())
			{
				dx::XMFLOAT3 clipPos = DXMath::LocalToClip(
					mTargetEnemy->GetWorldTransform(),
					renderer->GetViewMatrix(),
					renderer->GetProjectionMatrix()
				);

				// if target enemy is inside screen
				if (clipPos.y > -540.0f &&
					clipPos.y < 0.0f &&
					clipPos.x > -960.0f &&
					clipPos.x < 960.0f)
				{
					if (!mIsLockOnSE)
					{
						game->GetAudioSystem()->PlaySoundEx(index, XAUDIO2_LOOP_INFINITE);
						mIsLockOnSE = true;
					}

					sprite->SetVisible(true);
					sprite->GetOwner()->SetPosition(clipPos);
					stopLockOnSE = false;
				}
			}
		}

		if (stopLockOnSE)
		{
			game->GetAudioSystem()->StopSound(index);
			mIsLockOnSE = false;
		}

		if (mEmitterCD <= 0.0f)
		{
			EmissionActor* emission = new EmissionActor(GetScene());
			emission->SetPosition(GetPosition() - GetForward()*1.2f + Random::GetVector()*0.08f);
			emission->SetRotation(GetRotation());
			emission->SetScale(2.0f);
			mEmitterCD = 0.05f;
		}

		if (mIsInCloud && !mIsLockedOn)
		{
			mPlayerTranslucenceComp->SetVisible(true);
			mPlayerTranslucenceComp->GetOwner()->SetPosition(GetPosition());
			mPlayerTranslucenceComp->GetOwner()->SetRotation(GetRotation());
		}
		else
		{
			mPlayerTranslucenceComp->SetVisible(false);
		}
	}
}

float PlayerActor::GetForwardSpeed() const
{
	return mMoveComponent->GetForwardSpeed();
}

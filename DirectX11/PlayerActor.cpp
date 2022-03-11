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
#include "AudioSystem.h"
#include "Keyboard.h"
#include "GamePad.h"
#include "Collision.h"
#include "PhysWorld.h"
#include "DefineConstant.h"
#include "XMFloatHelper.h"
#include "Texture.h"
#include "Random.h"

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
	mOutCloudTime(14.0f),
	mEmitterCD(0.05f),
	mPlayerSprite(nullptr),
	mTargetEnemy(nullptr),
	mIsInCloud(false),
	mIsLockedOn(false),
	mOutCloudTimeMax(14.0f),
	mIsLockOnSE(false)
{
	SetScale(0.125f);
	SetPosition(dx::XMFLOAT3{ 0.0f,Constant::height,0.0f });

	Renderer* renderer = GetScene()->GetRenderer();	
	Mesh* mesh = renderer->GetMesh("player");
	mesh->ParseMesh(renderer, "player", L"Phong");
	MeshComponent* mc = new MeshComponent(this, mesh);

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

		mMoveComponent->SetAcceleration(accelW);
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

		mMoveComponent->SetAcceleration(accelS);
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

		mMoveComponent->SetAcceleration(accelNatural);
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
			if (mOutCloudTime > 0.0f)
			{
				mOutCloudTime += deltaTime * 2.0f;
			}
			if (mOutCloudTime > mOutCloudTimeMax)
			{
				mOutCloudTime = mOutCloudTimeMax;
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

		if (phys->IsCollidedWithEnemy(mBody, info))
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
				}
				else
				{
					game->GetAudioSystem()->StopSound(index);
					mIsLockOnSE = false;
				}
			}
			else
			{
				game->GetAudioSystem()->StopSound(index);
				mIsLockOnSE = false;
			}
		}
		else
		{
			game->GetAudioSystem()->StopSound(index);
			mIsLockOnSE = false;
		}

		if (mEmitterCD <= 0.0f)
		{
			EmitterActor* emitter = new EmitterActor(GetScene());
			emitter->SetPosition(GetPosition() - GetForward()*1.5f + Random::GetVector()*0.08f);
			emitter->SetRotation(GetRotation());
			emitter->SetScale(2.0f);
			mEmitterCD = 0.05f;
		}

		if (mIsInCloud && !mIsLockedOn)
		{
			Actor* actor = mPlayerSprite->GetOwner();
			mPlayerSprite->SetVisible(true);
			dx::XMFLOAT3 clipPos = DXMath::LocalToClip(
				this->GetWorldTransform(),
				renderer->GetViewMatrix(),
				renderer->GetProjectionMatrix()
			);

			actor->SetPosition(clipPos);
		}
		else
		{
			mPlayerSprite->SetVisible(false);
		}
	}
}

float PlayerActor::GetForwardSpeed() const
{
	return mMoveComponent->GetForwardSpeed();
}

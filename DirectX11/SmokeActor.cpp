#include "SmokeActor.h"
#include "Renderer.h"
#include "Mesh.h"
#include "TransparentComponent.h"
#include "GameScene.h"
#include "ConstantBuffer.h"
#include "Random.h"
#include "ImGui/imgui.h"
#include "DefineConstant.h"
#include "XMFloatHelper.h"

namespace dx = DirectX;

int SmokeActor::mCount = 0;
PixelConstantBuffer<SmokeActor::ObjectConstant>* SmokeActor::mObjectCBuffer = nullptr;
PixelConstantBuffer<SmokeActor::SmokeConstant>* SmokeActor::mSmokeCBuffer = nullptr;

SmokeActor::SmokeActor(BaseScene* scene, DirectX::XMFLOAT3 rotation)
	:
	Actor(scene),
	mIsAnimation(true),
	mAnimationSpeed(1.0f),
	mMissileRotation(rotation)
{
	mCount++;
	Reset();

	Renderer* renderer = GetScene()->GetRenderer();
	Mesh* mesh = renderer->GetMesh("cube");
	mesh->ParseMesh(renderer, "cube", L"RayMarching", false);
	TransparentComponent* tc = new TransparentComponent(this, mesh);

	if (!mObjectCBuffer)
	{
		mObjectCBuffer = new PixelConstantBuffer<ObjectConstant>(renderer, 1);
	}
	if (!mSmokeCBuffer)
	{
		mSmokeCBuffer = new PixelConstantBuffer<SmokeConstant>(renderer, 2);
	}
}

SmokeActor::~SmokeActor()
{
	mCount--;

	if (mCount == 0)
	{
		delete mObjectCBuffer;
		mObjectCBuffer = nullptr;
		delete mSmokeCBuffer;
		mSmokeCBuffer = nullptr;
	}
}

void SmokeActor::UpdateActor(float deltaTime)
{
	auto game = dynamic_cast<GameScene*>(GetScene());

	if (mIsAnimation)
	{
		float rate = 1.0f;
		mData.mLoop -= 32.0f * rate * mAnimationSpeed * deltaTime;
		mData.mAbsorption += 100.0f * rate * mAnimationSpeed * deltaTime;
		mData.mRadius += 0.2f * rate * mAnimationSpeed * deltaTime;

		if (mData.mLoop <= 0.0f || mData.mAbsorption >= 100.0f || mData.mRadius >= 0.2f)
		{
			if (game)
			{
				SetActorState(ActorState::EDead);
			}
			else
			{
				Reset();
			}
		}
	}

	if (game)
	{
		SetRotation(dx::XMFLOAT3{
			mMissileRotation.x + Constant::PI / 2.0f,
			mMissileRotation.y,
			mMissileRotation.z
		});
	}
}

void SmokeActor::Bind(Renderer* renderer)
{
	Actor::Bind(renderer);

	ObjectConstant c = {};
	c.mWorldTransformInverse = dx::XMMatrixInverse(nullptr, GetWorldTransform());
	c.mTime = GetScene()->GetGameTime();
	c.mType = 2;
	mObjectCBuffer->Update(renderer, c);
	mObjectCBuffer->Bind(renderer);

	mSmokeCBuffer->Update(renderer, mData);
	mSmokeCBuffer->Bind(renderer);
}

void SmokeActor::ImGuiWindow()
{
	if (ImGui::Begin("Ray Marching (Smoke)"))
	{
		ImGui::Text("Base");
		ImGui::ColorEdit3("Base Color", &mData.mColor.x);
		ImGui::SliderFloat("Absorption", &mData.mAbsorption, 0.0f, 100.0f, "%.1f");
		ImGui::SliderFloat("Opacity", &mData.mOpacity, 0.0f, 100.0f, "%.1f");
		ImGui::SliderFloat("Loop", &mData.mLoop, 0.0f, 64.0f, "%.f");

		ImGui::Text("Noise");
		ImGui::SliderFloat("Noise Scale", &mData.mNoiseScale, 0.0f, 64.0f, "%.2f");
		ImGui::SliderFloat("Radius", &mData.mRadius, 0.0f, 0.2f, "%.2f");

		ImGui::Text("Light");
		ImGui::SliderFloat("Absorption Light", &mData.mAbsorptionLight, 0.0f, 100.0f, "%.1f");
		ImGui::SliderFloat("Opacity Light", &mData.mOpacityLight, 0.0f, 100.0f, "%.1f");
		ImGui::SliderFloat("Light Step Scale", &mData.mLightStepScale, 0.0f, 1.0f, "%.2f");
		ImGui::SliderInt("Loop Light", &mData.mLoopLight, 0, 16, "%d");

		ImGui::Text("Animation");
		ImGui::SliderFloat("Speed", &mAnimationSpeed, 0.0f, 5.0f, "%.1f");
		ImGui::Checkbox("Enable", &mIsAnimation);

		if (ImGui::Button("Reset"))
		{
			Reset();
			mAnimationSpeed = 1.0f;
		}
	}
	ImGui::End();
}

void SmokeActor::Reset()
{
	mData = {
		{0.3f, 0.3f, 0.3f},
		32.0f,
		20.0f,
		0.0f,
		0.0f,
		100.0f,
		60.0f,
		80.0f,
		0.4f,
		4,
	};
}

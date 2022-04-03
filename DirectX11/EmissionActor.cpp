#include "EmissionActor.h"
#include "PlayerActor.h"
#include "Renderer.h"
#include "Mesh.h"
#include "TranslucenceComponent.h"
#include "GameScene.h"
#include "ConstantBuffer.h"
#include "Random.h"
#include "ImGui/imgui.h"
#include "DefineConstant.h"
#include "XMFloatHelper.h"

namespace dx = DirectX;

static float rate = 100.0f;

int EmissionActor::mCount = 0;
PixelConstantBuffer<EmissionActor::ObjectConstant>* EmissionActor::mObjectCBuffer = nullptr;
PixelConstantBuffer<EmissionActor::EmissionConstant>* EmissionActor::mEmissionCBuffer = nullptr;

EmissionActor::EmissionActor(BaseScene* scene)
	:
	Actor(scene),
	mIsAnimation(true),
	mAnimationSpeed(1.0f)
{
	mCount++;
	Reset();

	Renderer* renderer = GetScene()->GetRenderer();
	Mesh* mesh = renderer->GetMesh("cube");
	mesh->ParseMesh(renderer, "cube", L"RayMarching", false);
	TranslucenceComponent* tc = new TranslucenceComponent(this, mesh, 50);

	if (!mObjectCBuffer)
	{
		mObjectCBuffer = new PixelConstantBuffer<ObjectConstant>(renderer, 1);
	}
	if (!mEmissionCBuffer)
	{
		mEmissionCBuffer = new PixelConstantBuffer<EmissionConstant>(renderer, 2);
	}
}

EmissionActor::~EmissionActor()
{
	mCount--;

	if (mCount == 0)
	{
		delete mObjectCBuffer;
		mObjectCBuffer = nullptr;
		delete mEmissionCBuffer;
		mEmissionCBuffer = nullptr;
	}
}

void EmissionActor::UpdateActor(float deltaTime)
{
	if (mIsAnimation)
	{
		mData.mNoiseScale += rate * mAnimationSpeed * deltaTime;
		mData.mLoop -= 20.0f * mAnimationSpeed * deltaTime;

		if ((mData.mNoiseScale > 60.0f && rate > 0.0f) ||
			(mData.mNoiseScale < 40.0f && rate < 0.0f))
		{
			rate *= -1.0f;
		}

		if (mData.mLoop <= 0.0f)
		{
			if (auto game = dynamic_cast<GameScene*>(GetScene()))
			{
				SetActorState(ActorState::EDead);
			}
			else
			{
				Reset();
			}
		}
	}
}

void EmissionActor::Bind(Renderer* renderer)
{
	Actor::Bind(renderer);

	ObjectConstant c = {};
	c.mWorldTransformInverse = dx::XMMatrixInverse(nullptr, GetWorldTransform());
	c.mTime = GetScene()->GetGameTime();
	c.mType = 3;
	mObjectCBuffer->Update(renderer, c);
	mObjectCBuffer->Bind(renderer);

	mEmissionCBuffer->Update(renderer, mData);
	mEmissionCBuffer->Bind(renderer);
}

void EmissionActor::ImGuiWindow()
{
	if (ImGui::Begin("Ray Marching (Emission)"))
	{
		ImGui::Text("Base");
		ImGui::ColorEdit3("Base Color", &mData.mColor.x);
		ImGui::SliderFloat("Absorption", &mData.mAbsorption, 0.0f, 100.0f, "%.1f");
		ImGui::SliderFloat("Opacity", &mData.mOpacity, 0.0f, 100.0f, "%.1f");
		ImGui::SliderFloat("Loop", &mData.mLoop, 0.0f, 64.0f, "%.f");

		ImGui::Text("Noise");
		ImGui::SliderFloat("Noise Scale", &mData.mNoiseScale, 0.0f, 64.0f, "%.2f");
		ImGui::SliderFloat("Radius", &mData.mRadius, 0.0f, 1.0f, "%.2f");

		ImGui::Text("Light");
		ImGui::SliderFloat("Absorption Light", &mData.mAbsorptionLight, 0.0f, 100.0f, "%.1f");
		ImGui::SliderFloat("Opacity Light", &mData.mOpacityLight, 0.0f, 100.0f, "%.1f");
		ImGui::SliderFloat("Light Step Scale", &mData.mLightStepScale, 0.0f, 1.0f, "%.2f");
		ImGui::SliderInt("Loop Light", &mData.mLoopLight, 0, 16, "%d");

		ImGui::Text("Animation");
		ImGui::SliderFloat("Speed", &mAnimationSpeed, 0.0f, 2.0f, "%.1f");
		ImGui::Checkbox("Enable", &mIsAnimation);

		if (ImGui::Button("Reset"))
		{
			Reset();
			mAnimationSpeed = 1.0f;
		}
	}
	ImGui::End();
}

void EmissionActor::Reset()
{
	mData = {
		{0.3f, 0.3f, 1.0f},
		5.0f,
		20.0f,
		0.2f,
		40.0f,
		100.0f,
		60.0f,
		80.0f,
		0.4f,
		3,
	};
}

#include "EmitterActor.h"
#include "PlayerActor.h"
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

static float rate = 100.0f;

int EmitterActor::mCount = 0;
PixelConstantBuffer<EmitterActor::ObjectConstant>* EmitterActor::mObjectCBuffer = nullptr;
PixelConstantBuffer<EmitterActor::EmitterConstant>* EmitterActor::mEmitterCBuffer = nullptr;

EmitterActor::EmitterActor(BaseScene* scene)
	:
	Actor(scene),
	mIsAnimation(true)
{
	mCount++;
	Reset();

	Renderer* renderer = GetScene()->GetRenderer();
	Mesh* mesh = renderer->GetMesh("cube");
	mesh->ParseMesh(renderer, "cube", L"RayMarching", false);
	TransparentComponent* tc = new TransparentComponent(this, mesh, 50);

	if (!mObjectCBuffer)
	{
		mObjectCBuffer = new PixelConstantBuffer<ObjectConstant>(renderer, 1);
	}
	if (!mEmitterCBuffer)
	{
		mEmitterCBuffer = new PixelConstantBuffer<EmitterConstant>(renderer, 2);
	}
}

EmitterActor::~EmitterActor()
{
	mCount--;

	if (mCount == 0)
	{
		delete mObjectCBuffer;
		mObjectCBuffer = nullptr;
		delete mEmitterCBuffer;
		mEmitterCBuffer = nullptr;
	}
}

void EmitterActor::UpdateActor(float deltaTime)
{
	if (mIsAnimation)
	{
		mData.mNoiseScale += rate * deltaTime;
		mData.mLoop -= 20.0f * deltaTime;

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

void EmitterActor::Bind(Renderer* renderer)
{
	Actor::Bind(renderer);

	ObjectConstant c = {};
	c.mWorldTransformInverse = dx::XMMatrixInverse(nullptr, GetWorldTransform());
	c.mTime = GetScene()->GetGameTime();
	c.mType = 3;
	mObjectCBuffer->Update(renderer, c);
	mObjectCBuffer->Bind(renderer);

	mEmitterCBuffer->Update(renderer, mData);
	mEmitterCBuffer->Bind(renderer);
}

void EmitterActor::ImGuiWindow()
{
	if (ImGui::Begin("Ray Marching (Emitter)"))
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
		ImGui::Checkbox("Enable", &mIsAnimation);

		if (ImGui::Button("Reset"))
		{
			Reset();
		}
	}
	ImGui::End();
}

void EmitterActor::Reset()
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

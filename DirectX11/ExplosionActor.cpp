#include "ExplosionActor.h"
#include "Renderer.h"
#include "Mesh.h"
#include "TransparentComponent.h"
#include "SphereComponent.h"
#include "GameScene.h"
#include "ConstantBuffer.h"
#include "Random.h"
#include "ImGui/imgui.h"
#include "Collision.h"
#include "DefineConstant.h"
#include "PhysWorld.h"

namespace dx = DirectX;

int ExplosionActor::mCount = 0;
PixelConstantBuffer<ExplosionActor::ObjectConstant>* ExplosionActor::mObjectCBuffer = nullptr;
PixelConstantBuffer<ExplosionActor::ExplosionConstant>* ExplosionActor::mExplosionCBuffer = nullptr;

ExplosionActor::ExplosionActor(BaseScene* scene)
	:
	Actor(scene),
	mPhase(ExplosionPhase::EOne)
{	
	SetScale(10.0f);
	if (auto game = dynamic_cast<GameScene*>(GetScene()))
	{
	}
	else
	{
		SetPosition(dx::XMFLOAT3{ 5.0f,Constant::height,0.0f });
	}

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
	if (!mExplosionCBuffer)
	{
		mExplosionCBuffer = new PixelConstantBuffer<ExplosionConstant>(renderer, 2);
	}
}

ExplosionActor::~ExplosionActor()
{
	mCount--;

	if (mCount == 0)
	{
		delete mObjectCBuffer;
		mObjectCBuffer = nullptr;
		delete mExplosionCBuffer;
		mExplosionCBuffer = nullptr;
	}
}

void ExplosionActor::UpdateActor(float deltaTime)
{
	float rate = 1.5f;
	switch (mPhase)
	{
	case ExplosionActor::ExplosionPhase::EOne:
		mData.mRadius += 0.4f * rate * deltaTime;
		if (mData.mRadius >= 0.06f)
		{
			mData.mRadius = 0.06f;
			mPhase = ExplosionPhase::ETwo;
		}
		break;

	case ExplosionActor::ExplosionPhase::ETwo:
		mData.mLoop -= 16.0f * rate * deltaTime;
		mData.mAbsorptionLight -= 15.0f * rate * deltaTime;
		mData.mAbsorption += 50.0f * rate * deltaTime;
		mData.mRadius -= 0.015f * rate * deltaTime;

		if (mData.mLoop <= 0.0f || mData.mAbsorptionLight <= 0.0f || mData.mAbsorption >= 100.0f)
		{
			mData.mLoop = 0.0f;
			mData.mAbsorptionLight = 0.0f;
			mData.mAbsorption = 100.0f;
			if (auto game = dynamic_cast<GameScene*>(GetScene()))
			{
				SetActorState(ActorState::EDead);
			}
			else
			{
				mPhase = ExplosionPhase::EOne;
				Reset();
			}
		}

		break;
	}
}

void ExplosionActor::Bind(Renderer* renderer)
{
	Actor::Bind(renderer);

	ObjectConstant c = {};
	c.mWorldTransformInverse = dx::XMMatrixInverse(nullptr, GetWorldTransform());
	c.mTime = GetScene()->GetGameTime();
	c.mType = 1;
	mObjectCBuffer->Update(renderer, c);
	mObjectCBuffer->Bind(renderer);

	mExplosionCBuffer->Update(renderer, mData);
	mExplosionCBuffer->Bind(renderer);
}

void ExplosionActor::ImGuiWindow()
{
	if (ImGui::Begin("Ray Marching (Explosion)"))
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

		if (ImGui::Button("Reset"))
		{
			Reset();
		}
	}
	ImGui::End();
}

void ExplosionActor::Reset()
{
	mData = {
		{1.0f, 0.1f, 0.1f},
		32.0f,
		20.0f,
		0.0f,
		0.0f,
		100.0f,
		30.0f,
		80.0f,
		1.0f,
		4,
	};
}

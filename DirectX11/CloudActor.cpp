#include "CloudActor.h"
#include "Renderer.h"
#include "Mesh.h"
#include "TransparentComponent.h"
#include "SphereComponent.h"
#include "GameScene.h"
#include "DemoScene.h"
#include "ConstantBuffer.h"
#include "Random.h"
#include "ImGui/imgui.h"
#include "Collision.h"
#include "DefineConstant.h"
#include "PhysWorld.h"
#include "XMFloatHelper.h"

namespace dx = DirectX;

int CloudActor::mCount = 0;
CloudActor::CloudConstant CloudActor::mData;
PixelConstantBuffer<CloudActor::ObjectConstant>* CloudActor::mObjectCBuffer = nullptr;
PixelConstantBuffer<CloudActor::CloudConstant>* CloudActor::mCloudCBuffer = nullptr;

CloudActor::CloudActor(BaseScene* scene)
	:
	Actor(scene),
	mSpeed(1.0f),
	mIsAnimation(true)
{
	if (auto game = dynamic_cast<GameScene*>(GetScene()))
	{
		game->AddCloud(this);

		float size = Random::GetFloatRange(90.0f, 140.0f);
		float range = Constant::createRange;
		SetScale(dx::XMFLOAT3{ size,size / 5.0f,size });
		SetRotation(dx::XMFLOAT3{ 0.0f,Random::GetFloatRange(-Constant::PI,Constant::PI),0.0f });

		float radius = 0.3f;
		SphereComponent* sc = new SphereComponent(this);
		dx::XMFLOAT3 pos = dx::XMFLOAT3{ Random::GetFloatRange(-range,range),Constant::height,Random::GetFloatRange(-range,range) };
		Sphere* sphere = new Sphere(pos, radius * GetScale().x);
		sc->SetSphere(sphere);

		PhysWorld* phys = game->GetPhysWorld();
		while (phys->IsCollidedWithCloud(sc))
		{
			pos = dx::XMFLOAT3{ Random::GetFloatRange(-range,range),Constant::height,Random::GetFloatRange(-range,range) };
			sphere->mCenter = pos;
		}
		SetPosition(pos);
	}

	mCount++;
	Reset();

	Renderer* renderer = GetScene()->GetRenderer();
	Mesh* mesh = renderer->GetMesh("cube");
	mesh->ParseMesh(renderer, "cube", L"RayMarching", false);
	TransparentComponent* tc = new TransparentComponent(this, mesh, 200);

	if (!mObjectCBuffer)
	{
		mObjectCBuffer = new PixelConstantBuffer<ObjectConstant>(renderer, 1);
	}
	if (!mCloudCBuffer)
	{
		mCloudCBuffer = new PixelConstantBuffer<CloudConstant>(renderer, 2);
	}
}

CloudActor::~CloudActor()
{
	if (auto game = dynamic_cast<GameScene*>(GetScene()))
	{
		game->RemoveCloud(this);
	}

	mCount--;

	if (mCount == 0)
	{
		delete mObjectCBuffer;
		mObjectCBuffer = nullptr;
		delete mCloudCBuffer;
		mCloudCBuffer = nullptr;
	}
}

void CloudActor::UpdateActor(float deltaTime)
{
	if (auto demo = dynamic_cast<DemoScene*>(GetScene()))
	{
		SetRecomputeWorldTransform(true);
	}

	if (mIsAnimation)
	{
		mData.mNoiseScale = 10.0f + 1.5f * sin(GetScene()->GetGameTime()*mSpeed);
	}
}

void CloudActor::Bind(Renderer* renderer)
{
	Actor::Bind(renderer);

	ObjectConstant c = {};
	c.mWorldTransformInverse = dx::XMMatrixInverse(nullptr, GetWorldTransform());
	c.mTime = GetScene()->GetGameTime();
	c.mType = 0;
	mObjectCBuffer->Update(renderer, c);
	mObjectCBuffer->Bind(renderer);

	mCloudCBuffer->Update(renderer, mData);
	mCloudCBuffer->Bind(renderer);
}

void CloudActor::ImGuiWindow()
{
	if (ImGui::Begin("Ray Marching (Cloud)"))
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

		ImGui::Text("Scale");
		ImGui::SliderFloat("x", &GetScaleChange().x, 0.0f, 120.0f, "%.1f");
		ImGui::SliderFloat("y", &GetScaleChange().y, 0.0f, 20.0f, "%.1f");
		ImGui::SliderFloat("z", &GetScaleChange().z, 0.0f, 120.0f, "%.1f");

		ImGui::Text("Animation");
		ImGui::SliderFloat("Speed", &mSpeed, 0.0f, 10.0f, "%.1f");
		ImGui::Checkbox("Enable", &mIsAnimation);

		if (ImGui::Button("Reset"))
		{
			Reset();
			SetScale(dx::XMFLOAT3{ 10.0f,5.0f,10.0f });
			mSpeed = 1.0f;
		}
	}
	ImGui::End();
}

void CloudActor::Reset()
{
	mData = {
		{0.82f, 0.82f, 0.82f},
		32.0f,
		10.0f,
		0.7f,
		40.0f,
		70.0f,
		20.0f,
		30.0f,
		1.0f,
		3,
	};
}

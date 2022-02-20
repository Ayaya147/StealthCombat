#include "CloudActor.h"
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

int CloudActor::mCount = 0;
CloudActor::CloudConstant CloudActor::mData;
PixelConstantBuffer<CloudActor::ObjectConstant>* CloudActor::mObjectCBuffer = nullptr;
PixelConstantBuffer<CloudActor::CloudConstant>* CloudActor::mCloudCBuffer = nullptr;

CloudActor::CloudActor(BaseScene* scene)
	:
	Actor(scene),
	mDistFromCamera(0.0f)
{
	auto game = dynamic_cast<GameScene*>(GetScene());
	if (game)
	{
		game->AddCloud(this);

		float size = Random::GetFloatRange(90.0f, 140.0f);
		float range = Constant::createRange;
		SetScale(dx::XMFLOAT3{ size,size / 6.0f,size });
		SetRotation(dx::XMFLOAT3{ 0.0f,Random::GetFloatRange(-Constant::PI,Constant::PI),0.0f });

		float radius = 0.33f;
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
	else
	{
		float size = 100.0f;
		SetScale(dx::XMFLOAT3{ size,size / 6.0f,size });
		SetRotation(dx::XMFLOAT3{ 0.0f,Random::GetFloatRange(-Constant::PI,Constant::PI),0.0f });
		SetPosition(dx::XMFLOAT3{ 0.0f,Constant::height,45.0f });
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
	//mDistFromCamera = CalcDistFromCamera();
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

float CloudActor::CalcDistFromCamera()
{
	dx::XMMATRIX view = GetScene()->GetRenderer()->GetViewMatrix();
	dx::XMMATRIX matrix = GetWorldTransform() * view;
	float x = matrix.r[3].m128_f32[0];
	float y = matrix.r[3].m128_f32[1];
	float z = matrix.r[3].m128_f32[2];

	return x * x + y * y + z * z;
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

		if (ImGui::Button("Reset"))
		{
			Reset();
		}
	}
	ImGui::End();
}

void CloudActor::Reset()
{
	mData = {
		{0.85f, 0.85f, 0.85f},
		32.0f,
		10.0f,
		0.7f,
		60.0f,
		100.0f,
		60.0f,
		80.0f,
		0.4f,
		4,
	};
}

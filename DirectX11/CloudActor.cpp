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
	game->AddCloud(this);
	mCount++;
	Reset();

	Renderer* renderer = GetScene()->GetRenderer();
	Mesh* mesh = renderer->GetMesh("cube");
	mesh->ParseMesh(renderer, "cube", L"Raymarching", false);
	TransparentComponent* tc = new TransparentComponent(this, mesh);

	//float size = 140.0f;
	float size = Random::GetFloatRange(70.0f, 130.0f);
	float range = Constant::createRange;
	SetScale(dx::XMFLOAT3{ size,size / 6.0f,size });
	SetRotation(dx::XMFLOAT3{ 0.0f,Random::GetFloatRange(-Constant::PI,Constant::PI),0.0f });
	SetPosition(dx::XMFLOAT3{ Random::GetFloatRange(-range,range),Constant::height,Random::GetFloatRange(-range,range) });

	float radius = 0.33f;
	SphereComponent* sc = new SphereComponent(this);
	Sphere* sphere = new Sphere(GetPosition(), radius * GetScale().x);
	sc->SetSphere(sphere);

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
	if (ImGui::Begin("Ray Marching"))
	{
		ImGui::Text("Cloud");
		ImGui::ColorEdit3("Cloud Color", &mData.mCloudColor.x);
		ImGui::SliderInt("Absorption", &mData.mAbsorption, 0, 100, "%d");
		ImGui::SliderInt("Opacity", &mData.mOpacity, 0, 100, "%d");
		ImGui::SliderInt("Loop", &mData.mLoop, 0, 64, "%d");

		ImGui::Text("Noise");
		ImGui::SliderFloat("Noise Scale", &mData.mNoiseScale, 0, 64, "%.2f");
		ImGui::SliderFloat("Radius", &mData.mRadius, 0.0f, 1.0f, "%.2f");

		ImGui::Text("Light");
		ImGui::SliderInt("Absorption Light", &mData.mAbsorptionLight, 0, 100, "%d");
		ImGui::SliderInt("Opacity Light", &mData.mOpacityLight, 0, 100, "%d");
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
		32,
		10.0f,
		0.7f,
		60,
		100,
		60,
		80,
		0.4f,
		4,
	};
}

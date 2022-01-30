#include "CloudActor.h"
#include "Renderer.h"
#include "Mesh.h"
#include "TransparentComponent.h"
#include "TransformCBuffer.h"
#include "GameScene.h"
#include "ConstantBuffer.h"
#include "ImGui/imgui.h"

namespace dx = DirectX;

int CloudActor::mCount = 0;
PixelConstantBuffer<CloudActor::ObjectConstant>* CloudActor::mObjectCBuffer = nullptr;
PixelConstantBuffer<CloudActor::CloudConstant>* CloudActor::mCloudCBuffer = nullptr;

CloudActor::CloudActor(BaseScene* scene)
	:
	Actor(scene),
	mDistFromCamera(0.0f)
{
	mCount++;

	Reset();
	Renderer* renderer = GetScene()->GetRenderer();
	SetTransformCBuffer(new TransformCBuffer(renderer, this));

	Mesh* mesh = renderer->GetMesh("cube");
	mesh->ParseMesh(renderer, "cube", L"Raymarching", false);
	TransparentComponent* tc = new TransparentComponent(this, mesh);

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
	mCount--;

	if (mCount == 0)
	{
		delete mObjectCBuffer;
		delete mCloudCBuffer;
	}
}

void CloudActor::UpdateActor(float deltaTime)
{
	mData.mTime = GetScene()->GetGameTime();
	mDistFromCamera = CalcDistFromCamera();
}

void CloudActor::Bind(Renderer* renderer)
{
	Actor::Bind(renderer);

	ObjectConstant c = {};
	c.mWorldTransformInverse = dx::XMMatrixInverse(nullptr, GetWorldTransform());
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

void CloudActor::ImGuiWinodow()
{
	if (ImGui::Begin("Ray Marching"))
	{
		ImGui::Text("Base");
		ImGui::ColorEdit3("Cloud Color", &mData.mCloudColor.x);
		ImGui::SliderInt("Absorption", &mData.mAbsorption, 0, 100, "%d");
		ImGui::SliderInt("Opacity", &mData.mOpacity, 0, 100, "%d");
		ImGui::SliderInt("Loop", &mData.mLoop, 0, 64, "%d");

		ImGui::Text("Noise");
		ImGui::SliderFloat("Noise Scale", &mData.mNoiseScale, 0, 64, "%.1f");
		ImGui::SliderFloat("Radius", &mData.mRadius, 0.0f, 2.0f, "%.2f");

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
		{0.9f, 0.9f, 0.9f},
		32,
		8.0f,
		0.65f,
		50,
		100,
		60,
		80,
		0.4f,
		6,
		GetScene()->GetGameTime()
	};
}

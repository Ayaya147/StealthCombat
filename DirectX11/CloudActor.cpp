#include "CloudActor.h"
#include "Renderer.h"
#include "Mesh.h"
#include "MeshComponent.h"
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
	Actor(scene)
{
	mCount++;

	Reset();
	Renderer* renderer = GetScene()->GetRenderer();
	SetTransformCBuffer(new TransformCBuffer(renderer, this));

	Mesh* mesh = renderer->GetMesh("cube", L"Raymarching");
	mesh->ParseMesh(renderer, "cube", L"Raymarching");
	MeshComponent* mc = new MeshComponent(this, mesh);

	if (!mObjectCBuffer)
	{
		mObjectCBuffer = new PixelConstantBuffer<ObjectConstant>(renderer, 2);
	}
	if (!mCloudCBuffer)
	{
		mCloudCBuffer = new PixelConstantBuffer<CloudConstant>(renderer, 3);
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
		ImGui::SliderInt("Noise Scale", &mData.mNoiseScale, 0, 64, "%d");
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
		8,
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

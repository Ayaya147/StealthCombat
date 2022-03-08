#include "PlaneActor.h"
#include "PlaneMesh.h"
#include "MeshComponent.h"
#include "MoveComponent.h"
#include "BaseScene.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "DxException.h"
#include "Random.h"
#include "ConstantBuffer.h"
#include "ImGui/imgui.h"

namespace dx = DirectX;

PlaneActor::PlaneActor(BaseScene* scene)
	:
	Actor(scene)
{
	Reset();

	Renderer* renderer = GetScene()->GetRenderer();
	Mesh* mesh = renderer->GetMesh("plane");
	PlaneMesh* planeMesh = dynamic_cast<PlaneMesh*>(mesh);
	planeMesh->ParseMesh(renderer, "plane", L"GerstnerWave", 101, 20.0f);
	MeshComponent* mc = new MeshComponent(this, planeMesh);
	mCount = planeMesh->GetVerticesCount();

	for (auto b : mesh->GetBindables())
	{
		if (auto vertexBuffer = dynamic_cast<VertexBuffer*>(b))
		{
			mVertexBuffer = vertexBuffer;
		}
	}

	mVertexCBuffer = new VertexConstantBuffer<VertexConstant>(renderer, 1);
	mPixelCBuffer = new PixelConstantBuffer<PixelConstant>(renderer, 1);
}

PlaneActor::~PlaneActor()
{
	delete mVertexCBuffer;
	delete mPixelCBuffer;
}

void PlaneActor::UpdateActor(float deltaTime)
{
	mPixelData.mTime = GetScene()->GetGameTime();
	mVertexData.mTime = mPixelData.mTime;
	mVertexData.mNoiseSizeLerp = mPixelData.mNoiseSizeLerp;
	mVertexData.mNoiseStrength = mPixelData.mNoiseStrength;
	mVertexData.mWaveSpeed = mPixelData.mWaveSpeed;
}

void PlaneActor::Bind(Renderer* renderer)
{
	Actor::Bind(renderer);

	mVertexCBuffer->Update(renderer, mVertexData);
	mVertexCBuffer->Bind(renderer);
	
	mPixelCBuffer->Update(renderer, mPixelData);
	mPixelCBuffer->Bind(renderer);
}

void PlaneActor::ImGuiWindow()
{
	if (ImGui::Begin("Gerstner Wave (Sea)"))
	{
		ImGui::Text("Color");
		ImGui::ColorEdit3("Sea Base Color", &mPixelData.mSeaBaseColor.x);
		ImGui::ColorEdit3("Sea Shallow Color", &mPixelData.mSeaShallowColor.x);
		ImGui::ColorEdit3("Sky Color", &mPixelData.mSkyColor.x);
		ImGui::SliderFloat("Base Color Strength", &mPixelData.mBaseColorStrength, 0.0f, 2.0f, "%.2f");
		ImGui::SliderFloat("Shallow Color Strength", &mPixelData.mShallowColorStrength, 0.0f, 1.0f, "%.2f");
		ImGui::SliderFloat("Color Offset", &mPixelData.mColorHeightOffset, 0.0f, 1.0f, "%.2f");

		ImGui::Text("Noise");
		ImGui::SliderFloat("Noise Strength", &mPixelData.mNoiseStrength, 0.0f, 5.0f, "%.2f");
		ImGui::SliderFloat("Noise Size", &mPixelData.mNoiseSizeLerp, 0.0f, 1.0f, "%.2f");

		ImGui::Text("Other");
		ImGui::SliderFloat("Wave Speed", &mPixelData.mWaveSpeed, 0.0f, 5.0f, "%.2f");
		ImGui::SliderFloat("Shininess", &mPixelData.mShininess, 0.0f, 1.0f, "%.2f");
		ImGui::SliderFloat("f0 (fresnel)", &mPixelData.mF0, 0.0f, 1.0f, "%.2f");

		if (ImGui::Button("Reset"))
		{
			Reset();
		}
	}
	ImGui::End();
}

void PlaneActor::Reset()
{
	mVertexData = {
		GetScene()->GetGameTime(),
		1.7f,
		0.75f,
		1.0f
	};

	mPixelData = {
		{0.05f, 0.19f, 0.35f},
		{0.36f, 0.54f, 0.19f},
		{0.0f, 0.5f, 0.8f},
		GetScene()->GetGameTime(),
		1.7f,
		0.75f,
		0.8f,
		0.80f,
		0.25f,
		0.75f,
		0.12f,
		0.20f
	};
}

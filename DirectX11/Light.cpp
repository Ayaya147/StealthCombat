#include "Light.h"
#include "ConstantBuffer.h"
#include "ImGui/imgui.h"

namespace dx = DirectX;

Light::Light(Renderer* renderer)
{
	Reset();
	mPixelCBuffer = new PixelConstantBuffer<PixelConstant>(renderer, 0);
}

Light::~Light()
{
	delete mPixelCBuffer;
}

void Light::Bind(Renderer* renderer)
{
	dx::XMMATRIX invView = dx::XMMatrixInverse(nullptr, renderer->GetViewMatrix());
	dx::XMStoreFloat3(&mData.mCameraPos, invView.r[3]);

	mPixelCBuffer->Update(renderer, mData);
	mPixelCBuffer->Bind(renderer);
}

void Light::Reset()
{
	mData = {
		mData.mCameraPos,
		{0.0f,-1.0f,-1.0f},
		{0.05f,0.05f,0.05f},
		{1.0f,1.0f,1.0f},
		{0.9f,0.9f,0.9f}
	};
}

void Light::ImGuiWindow()
{
	if (ImGui::Begin("Directional Light"))
	{
		ImGui::ColorEdit3("Ambient Color", &mData.mAmbientColor.x);
		ImGui::ColorEdit3("Diffuse Color", &mData.mDiffuseColor.x);
		ImGui::ColorEdit3("Specular Color", &mData.mSpecColor.x);

		if (ImGui::Button("Reset"))
		{
			Reset();
		}
	}
	ImGui::End();
}

void Light::SetDirectionalLight(const DirectionalLight& light)
{
	mData.mDirection = light.mDirection;
	mData.mAmbientColor = light.mAmbientColor;
	mData.mDiffuseColor = light.mDiffuseColor;
	mData.mSpecColor = light.mSpecColor;
}

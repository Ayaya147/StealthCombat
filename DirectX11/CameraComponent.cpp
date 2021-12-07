#include "CameraComponent.h"
#include "Actor.h"
#include "BaseScene.h"
#include "SceneManager.h"
#include "Renderer.h"

namespace dx = DirectX;

CameraComponent::CameraComponent(Actor* owner, int updateOrder)
	:
	Component(owner, updateOrder)
{
}

void CameraComponent::Update(float deltaTime)
{
	const auto cameraPos = dx::XMVector3Transform(
		dx::XMLoadFloat3(&mOwner->GetPosition()),
		dx::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f)
	);

	dx::XMMATRIX view = dx::XMMatrixLookAtLH(
		cameraPos,
		dx::XMVectorZero(),
		dx::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
	) * dx::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);

	mOwner->GetScene()->GetSceneManager()->GetRenderer()->SetViewMatrix(view);
}

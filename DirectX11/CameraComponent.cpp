#include "CameraComponent.h"
#include "Actor.h"
#include "BaseScene.h"
#include "SceneManager.h"
#include "Renderer.h"

namespace dx = DirectX;

CameraComponent::CameraComponent(Actor* owner, int updateOrder)
	:
	Component(owner, updateOrder),
	mTargetDist(15.0f)
{
}

void CameraComponent::Update(float deltaTime)
{
	dx::XMFLOAT3 cameraPos = dx::XMFLOAT3{ 
		mOwner->GetPosition().x,
		mOwner->GetPosition().y + mTargetDist,
		mOwner->GetPosition().z 
	};

	dx::XMFLOAT3 forward = mOwner->GetForward();
	dx::XMMATRIX view = dx::XMMatrixLookAtLH(
		dx::XMLoadFloat3(&cameraPos),
		dx::XMLoadFloat3(&mOwner->GetPosition()),
		dx::XMLoadFloat3(&forward)
	);

	mOwner->GetScene()->GetRenderer()->SetViewMatrix(view);
}

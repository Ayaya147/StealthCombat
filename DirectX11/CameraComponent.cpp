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
#if 0
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
#else
	dx::XMFLOAT3 forward = mOwner->GetForward();
	dx::XMFLOAT3 up = { 0.0f,1.0f,0.0f };
	dx::XMFLOAT3 cameraPos = dx::XMFLOAT3{
		mOwner->GetPosition().x - forward.x * mTargetDist,
		mOwner->GetPosition().y + 5.0f,
		mOwner->GetPosition().z - forward.z * mTargetDist
	};

	dx::XMMATRIX view = dx::XMMatrixLookAtLH(
		dx::XMLoadFloat3(&cameraPos),
		dx::XMLoadFloat3(&mOwner->GetPosition()),
		dx::XMLoadFloat3(&up)
	);
#endif
	mOwner->GetScene()->GetRenderer()->SetViewMatrix(view);

}

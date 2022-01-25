#include "CameraComponent.h"
#include "Actor.h"
#include "BaseScene.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "InputSystem.h"
#include "Keyboard.h"

namespace dx = DirectX;

CameraComponent::CameraComponent(Actor* owner, int updateOrder)
	:
	Component(owner, updateOrder),
	mTargetDist(15.0f),
	mType(1)
{
}

void CameraComponent::Update(float deltaTime)
{
	dx::XMMATRIX view;
	if (mType == 1)
	{
		dx::XMFLOAT3 cameraPos = dx::XMFLOAT3{
			GetOwner()->GetPosition().x,
			GetOwner()->GetPosition().y + mTargetDist,
			GetOwner()->GetPosition().z
		};

		dx::XMFLOAT3 forward = GetOwner()->GetForward();
		view = dx::XMMatrixLookAtLH(
			dx::XMLoadFloat3(&cameraPos),
			dx::XMLoadFloat3(&GetOwner()->GetPosition()),
			dx::XMLoadFloat3(&forward)
		);
	}
	else
	{
		dx::XMFLOAT3 forward = GetOwner()->GetForward();
		dx::XMFLOAT3 up = { 0.0f,1.0f,0.0f };
		dx::XMFLOAT3 cameraPos = dx::XMFLOAT3{
			GetOwner()->GetPosition().x - forward.x * mTargetDist * 0.9f,
			GetOwner()->GetPosition().y + 4.0f,
			GetOwner()->GetPosition().z - forward.z * mTargetDist * 0.9f
		};

		view = dx::XMMatrixLookAtLH(
			dx::XMLoadFloat3(&cameraPos),
			dx::XMLoadFloat3(&GetOwner()->GetPosition()),
			dx::XMLoadFloat3(&up)
		);
	}

	GetOwner()->GetScene()->GetRenderer()->SetViewMatrix(view);

}

void CameraComponent::ProcessInput()
{
	Keyboard* keyboard = GetOwner()->GetScene()->GetInputSystem()->GetKeyboard();

	if (keyboard->KeyIsPressed('1'))
	{
		mType = 1;
	}
	else if (keyboard->KeyIsPressed('2'))
	{
		mType = 2;
	}
}

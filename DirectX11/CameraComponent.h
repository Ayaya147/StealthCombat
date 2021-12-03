#pragma once
#include "Component.h"
#include <DirectXMath.h>

class CameraComponent : public Component
{
public:
	CameraComponent(class Actor* owner, int updateOrder = 200);

	void Update(float deltaTime) override;

private:
	void SetViewMatrix(const DirectX::XMMATRIX& view);
};

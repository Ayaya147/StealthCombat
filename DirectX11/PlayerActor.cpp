#include "PlayerActor.h"
#include "BaseScene.h"
#include "MeshComponent.h"
#include "MoveComponent.h"
#include "Mesh.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "TransformCBuffer.h"

PlayerActor::PlayerActor(BaseScene* scene, const std::string& fileName)
	:
	Actor(scene)
{
	SetTransformCBuffer(new TransformCBuffer(scene->GetSceneManager()->GetRenderer(), this));
	SetScale(1.0f);

	Mesh* mesh = GetScene()->GetSceneManager()->GetRenderer()->GetMesh(fileName, L"Phong");
	MeshComponent* mc = new MeshComponent(this, mesh);

	MoveComponent* move = new MoveComponent(this);
	move->SetAngularSpeed(3.0f);
}

void PlayerActor::UpdateActor(float deltaTime)
{
	//DirectX::XMFLOAT3 rotation = GetRotation();
	//SetRotation(DirectX::XMFLOAT3{ rotation.x + 3.0f * deltaTime,rotation.y,rotation.z });
}

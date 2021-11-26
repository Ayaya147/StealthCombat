#include "PlayerActor.h"
#include "BaseScene.h"
#include "MeshComponent.h"
#include "Mesh.h"
#include "SceneManager.h"
#include "GameApp.h"
#include "Renderer.h"
#include "TransformCBuffer.h"

PlayerActor::PlayerActor(BaseScene* scene, const std::string& fileName)
	:
	Actor(scene)
{

	SetTransformCBuffer(new TransformCBuffer(scene->GetSceneManager()->GetApp()->GetRenderer(), this));

	Mesh* mesh = GetScene()->GetSceneManager()->GetApp()->GetRenderer()->GetMesh(fileName, L"Phong");
	MeshComponent* mc = new MeshComponent(this, mesh);

	SetScale(1.0f);
}

void PlayerActor::UpdateActor(float deltaTime)
{
	DirectX::XMFLOAT3 rotation = GetRotation();
	SetRotation(DirectX::XMFLOAT3{ rotation.x + 3.0f * deltaTime,rotation.y,rotation.z });
}

#include "PlayerActor.h"
#include "BaseScene.h"
#include "MeshComponent.h"
#include "Mesh.h"
#include "SceneManager.h"
#include "GameApp.h"
#include "Renderer.h"

PlayerActor::PlayerActor(BaseScene* scene, const std::string& fileName)
	:
	Actor(scene)
{
	Mesh* mesh = GetScene()->GetSceneManager()->GetApp()->GetRenderer()->GetMesh(fileName);
	MeshComponent* mc = new MeshComponent(this, mesh);

	mScale = 1.0f;
}

void PlayerActor::UpdateActor(float deltaTime)
{
	mRotation.x += 3.0f * deltaTime;
}

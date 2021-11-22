#include "PlayerActor.h"
#include "BaseScene.h"
#include "MeshComponent.h"
#include "Mesh.h"
#include "SceneManager.h"
#include "GameApp.h"
#include "Renderer.h"

PlayerActor::PlayerActor(BaseScene* scene)
	:
	Actor(scene)
{
	Mesh* mesh = GetScene()->GetSceneManager()->GetApp()->GetRenderer()->GetMesh("Assets\\Models\\test.obj");
	MeshComponent* mc = new MeshComponent(this, mesh);

	mPosition = { 0.0f,0.0f,5.0f };
	mScale = 1.0f;
}

void PlayerActor::UpdateActor(float deltaTime)
{
	mRotation.x += 3.0f * deltaTime;
}

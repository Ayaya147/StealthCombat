#include "PlayerActor.h"
#include "AbstractScene.h"
#include "MeshComponent.h"
#include "Mesh.h"
#include "SceneManager.h"
#include "GameApp.h"
#include "Renderer.h"

PlayerActor::PlayerActor(class AbstractScene* scene)
	:
	Actor(scene)
{
	MeshComponent* mc = new MeshComponent(this);
	Mesh* mesh = GetScene()->GetSceneManager()->GetApp()->GetRenderer()->GetMesh("Assets\\Models\\test.obj");
	mc->SetMesh(mesh);

	mPosition = { 0.0f,0.0f,5.0f };
	mScale = 1.0f;
}

void PlayerActor::UpdateActor(float deltaTime)
{
	mRotation.x += 0.05f;
}

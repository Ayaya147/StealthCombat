#include "PlayerActor.h"
#include "GameScene.h"
#include "MeshComponent.h"
#include "Mesh.h"
#include "SceneManager.h"
#include "GameApp.h"
#include "Renderer.h"

PlayerActor::PlayerActor(GameScene* game)
	:
	Actor(game)
{
	MeshComponent* mc = new MeshComponent(this);
	Mesh* mesh = GetGame()->GetSceneManager()->GetApp()->GetRenderer()->GetMesh("Assets\\Models\\test.obj");
	mc->SetMesh(mesh);

	mPosition = { 0.0f,0.0f,5.0f };
	mScale = 1.0f;
}

void PlayerActor::UpdateActor()
{
	mRotation.x += 0.05f;
}

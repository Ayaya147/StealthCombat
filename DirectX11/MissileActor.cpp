#include "MissileActor.h"
#include "BaseScene.h"
#include "MeshComponent.h"
#include "MoveComponent.h"
#include "Mesh.h"
#include "Renderer.h"

MissileActor::MissileActor(BaseScene* scene)
	:
	Actor(scene)
{
	SetScale(0.08f);

	Renderer* renderer = GetScene()->GetRenderer();
	Mesh* mesh = renderer->GetMesh("missile");
	mesh->ParseMesh(renderer, "missile", L"Phong");
	MeshComponent* mc = new MeshComponent(this, mesh);

	mMoveComponent = new MoveComponent(this);
	//mMoveComponent->SetAngularSpeed(1.0f);
}

void MissileActor::UpdateActor(float deltaTime)
{
}

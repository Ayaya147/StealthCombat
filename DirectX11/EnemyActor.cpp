#include "EnemyActor.h"
#include "BaseScene.h"
#include "MeshComponent.h"
#include "MoveComponent.h"
#include "Mesh.h"
#include "Renderer.h"
#include "TransformCBuffer.h"

EnemyActor::EnemyActor(BaseScene* scene)
	:
	Actor(scene)
{
	Renderer* renderer = GetScene()->GetRenderer();
	SetScale(0.1f);

	SetTransformCBuffer(new TransformCBuffer(renderer, this));

	Mesh* mesh = renderer->GetMesh("enemy");
	mesh->ParseMesh(renderer, "enemy", L"Phong");
	MeshComponent* mc = new MeshComponent(this, mesh);

	mMoveComponent = new MoveComponent(this);
	//mMoveComponent->SetAngularSpeed(1.0f);
}

void EnemyActor::UpdateActor(float deltaTime)
{
}

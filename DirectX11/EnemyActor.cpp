#include "EnemyActor.h"
#include "BaseScene.h"
#include "MeshComponent.h"
#include "MoveComponent.h"
#include "Mesh.h"
#include "Renderer.h"
#include "SphereComponent.h"
#include "Collision.h"

EnemyActor::EnemyActor(BaseScene* scene)
	:
	Actor(scene)
{
	SetScale(0.1f);
	SetPosition(DirectX::XMFLOAT3{ 0.0f,150.0f,5.0f });

	Renderer* renderer = GetScene()->GetRenderer();
	Mesh* mesh = renderer->GetMesh("enemy");
	mesh->ParseMesh(renderer, "enemy", L"Phong");
	MeshComponent* mc = new MeshComponent(this, mesh);

	mMoveComponent = new MoveComponent(this);
	//mMoveComponent->SetAngularSpeed(1.0f);

	float radius = 10.0f;
	SphereComponent* sc = new SphereComponent(this);
	Sphere* sphere = new Sphere(GetPosition(), radius * GetScale().x);
	sc->SetSphere(sphere);
}

void EnemyActor::UpdateActor(float deltaTime)
{
}

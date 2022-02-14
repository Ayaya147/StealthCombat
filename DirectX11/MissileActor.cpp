#include "MissileActor.h"
#include "BaseScene.h"
#include "MeshComponent.h"
#include "MoveComponent.h"
#include "Mesh.h"
#include "Renderer.h"
#include "SphereComponent.h"
#include "Collision.h"
#include "DefineConstant.h"

MissileActor::MissileActor(BaseScene* scene)
	:
	Actor(scene)
{
	SetScale(0.08f);
	SetPosition(DirectX::XMFLOAT3{ 0.0f,Constant::height,-8.0f });

	Renderer* renderer = GetScene()->GetRenderer();
	Mesh* mesh = renderer->GetMesh("missile");
	mesh->ParseMesh(renderer, "missile", L"Phong");
	MeshComponent* mc = new MeshComponent(this, mesh);

	mMoveComponent = new MoveComponent(this);
	//mMoveComponent->SetAngularSpeed(1.0f);

	float radius = 8.0f;
	SphereComponent* sc = new SphereComponent(this);
	Sphere* sphere = new Sphere(GetPosition(), radius * GetScale().x);
	sc->SetSphere(sphere);
	sphere = new Sphere(GetPosition(), radius * GetScale().x);
	sc->SetSphereLast(sphere);
}

void MissileActor::UpdateActor(float deltaTime)
{
}

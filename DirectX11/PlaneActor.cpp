#include "PlaneActor.h"
#include "TransformCBufferDouble.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "MoveComponent.h"
#include "BaseScene.h"
#include "SceneManager.h"
#include "Renderer.h"

PlaneActor::PlaneActor(BaseScene* scene)
	:
	Actor(scene)
{
	mBuffer = new TransformCBuffer(scene->GetSceneManager()->GetRenderer(), this, 0);
	SetScale(1.0f);

	Mesh* mesh = GetScene()->GetSceneManager()->GetRenderer()->GetMesh("plane", L"Phong");
	//Mesh* mesh = GetScene()->GetSceneManager()->GetRenderer()->GetMesh("plane", L"PhongNormalMap");
	MeshComponent* mc = new MeshComponent(this, mesh);

	//MoveComponent* move = new MoveComponent(this);
	//move->SetAngularSpeed(1.0f);
}

PlaneActor::~PlaneActor()
{
	delete mBuffer;
}

void PlaneActor::UpdateActor(float deltaTime)
{
}

void PlaneActor::Bind(Renderer* renderer)
{
	mBuffer->Bind(renderer);
}

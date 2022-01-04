#include "PlaneActor.h"
#include "TransformCBuffer.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "BaseScene.h"
#include "SceneManager.h"
#include "Renderer.h"

PlaneActor::PlaneActor(BaseScene* scene)
	:
	Actor(scene)
{
	SetTransformCBuffer(new TransformCBuffer(scene->GetSceneManager()->GetRenderer(), this));
	SetScale(1.0f);

	Mesh* mesh = GetScene()->GetSceneManager()->GetRenderer()->GetMesh("plane", L"Phong");
	MeshComponent* mc = new MeshComponent(this, mesh);
}

void PlaneActor::UpdateActor(float deltaTime)
{
}

void PlaneActor::Bind(Renderer* renderer)
{
	Actor::Bind(renderer);
}

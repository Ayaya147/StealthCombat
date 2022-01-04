#include "PlaneActor.h"
#include "TransformCBufferDouble.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "BaseScene.h"
#include "SceneManager.h"
#include "Renderer.h"

PlaneActor::PlaneActor(BaseScene* scene)
	:
	Actor(scene)
{
	mBuffer = new TransformCBufferDouble(scene->GetSceneManager()->GetRenderer(), this, 2);
	SetScale(1.0f);

	//Mesh* mesh = GetScene()->GetSceneManager()->GetRenderer()->GetMesh("plane", L"Phong");
	Mesh* mesh = GetScene()->GetSceneManager()->GetRenderer()->GetMesh("plane", L"PhongNormalMap");
	MeshComponent* mc = new MeshComponent(this, mesh);
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

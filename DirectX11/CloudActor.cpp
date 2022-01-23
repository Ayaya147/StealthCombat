#include "CloudActor.h"
#include "Renderer.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "TransformCBuffer.h"
#include "GameScene.h"
#include "ConstantBuffer.h"

namespace dx = DirectX;

CloudActor::CloudActor(BaseScene* scene)
	:
	Actor(scene)
{
	Renderer* renderer = GetScene()->GetRenderer();

	SetTransformCBuffer(new TransformCBuffer(renderer, this));

	Mesh* mesh = renderer->GetMesh("cube1", L"Raymarching");
	mesh->ParseMesh(renderer, "cube1", L"Raymarching");
	MeshComponent* mc = new MeshComponent(this, mesh);

	mObjectCBuffer = new PixelConstantBuffer<ObjectConstant>(renderer, 2);
}

CloudActor::~CloudActor()
{
	delete mObjectCBuffer;
}

void CloudActor::Bind(Renderer* renderer)
{
	Actor::Bind(renderer);

	ObjectConstant c = {};
	c.mWorldTransformInverse = dx::XMMatrixInverse(nullptr, GetWorldTransform());

	mObjectCBuffer->Update(renderer, c);
	mObjectCBuffer->Bind(renderer);
}

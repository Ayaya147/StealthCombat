#include "CloudActor.h"
#include "Renderer.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "TransformCBuffer.h"
#include "GameScene.h"
#include "ConstantBuffer.h"

namespace dx = DirectX;

CloudActor::CloudActor(BaseScene* scene, const std::string& fileName)
	:
	Actor(scene)
{
	Renderer* renderer = GetScene()->GetRenderer();

	SetTransformCBuffer(new TransformCBuffer(renderer, this));
	//SetScale(0.1f);

	Mesh* mesh = renderer->GetMesh(fileName, L"Raymarching", 1);
	//Mesh* mesh = renderer->GetMesh(fileName, L"Raymarching", 1);
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

#include "PlaneActor.h"
#include "TransformCBuffer.h"
#include "PlaneMesh.h"
#include "MeshComponent.h"
#include "MoveComponent.h"
#include "BaseScene.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "DxException.h"
#include "Random.h"
#include "ConstantBuffer.h"

namespace dx = DirectX;

PlaneActor::PlaneActor(BaseScene* scene)
	:
	Actor(scene)
{
	Renderer* renderer = GetScene()->GetRenderer();

	SetTransformCBuffer(new TransformCBuffer(renderer, this));
	SetScale(1.0f);

	Mesh* mesh = renderer->GetMesh("plane");
	PlaneMesh* planeMesh = dynamic_cast<PlaneMesh*>(mesh);
	planeMesh->ParseMesh(renderer, "plane", L"Sea",10 ,50.0f);
	MeshComponent* mc = new MeshComponent(this, planeMesh);

	mCount = planeMesh->GetVerticesCount();

	for (auto b : mesh->GetBindables())
	{
		if (auto vertexBuffer = dynamic_cast<VertexBuffer*>(b))
		{
			mVertexBuffer = vertexBuffer;
		}
	}


	//mObjectCBuffer1 = new VertexConstantBuffer<ObjectConstant>(renderer, 1);
	//mObjectCBuffer2 = new PixelConstantBuffer<ObjectConstant>(renderer, 1);

}

PlaneActor::~PlaneActor()
{
	//delete mObjectCBuffer1;
	//delete mObjectCBuffer2;
}

void PlaneActor::UpdateActor(float deltaTime)
{
	Renderer* renderer = GetScene()->GetRenderer();

	D3D11_MAPPED_SUBRESOURCE msr;
	renderer->GetContext()->Map(mVertexBuffer->GetVertexBuffer(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &msr);
	PlaneMesh::PlaneVertex* vertex = static_cast<PlaneMesh::PlaneVertex*>(msr.pData);

	for (int i = 0; i < mCount; i++)
	{
		vertex[i].tc0 = dx::XMFLOAT2{ vertex[i].tc0.x - 0.01f * deltaTime, vertex[i].tc0.y + 0.02f * deltaTime };
		vertex[i].tc1 = dx::XMFLOAT2{ vertex[i].tc1.x + 0.04f * deltaTime, vertex[i].tc1.y - 0.02f * deltaTime };
	}

	renderer->GetContext()->Unmap(mVertexBuffer->GetVertexBuffer(), 0);
}

void PlaneActor::Bind(Renderer* renderer)
{
	Actor::Bind(renderer);
	//ObjectConstant mData;
	//mData.time = GetScene()->GetGameTime();

	//mObjectCBuffer1->Update(renderer, mData);
	//mObjectCBuffer1->Bind(renderer);	
	//
	//mObjectCBuffer2->Update(renderer, mData);
	//mObjectCBuffer2->Bind(renderer);
}

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

namespace dx = DirectX;

PlaneActor::PlaneActor(BaseScene* scene)
	:
	Actor(scene)
{
	Renderer* renderer = GetScene()->GetRenderer();

	SetTransformCBuffer(new TransformCBuffer(renderer, this));
	SetScale(1.0f);

	Mesh* mesh = renderer->GetMesh("plane", L"PhongNormalMap");
	PlaneMesh* planeMesh = dynamic_cast<PlaneMesh*>(mesh);
	planeMesh->ParseMesh(renderer, "plane", L"PhongNormalMap",3 ,50.0f);
	MeshComponent* mc = new MeshComponent(this, planeMesh);

	mCount = planeMesh->GetVerticesCount();

	for (auto b : mesh->GetBindables())
	{
		if (auto vertexBuffer = dynamic_cast<VertexBuffer*>(b))
		{
			mVertexBuffer = vertexBuffer;
		}
	}
}

PlaneActor::~PlaneActor()
{
}

void PlaneActor::UpdateActor(float deltaTime)
{
	//Renderer* renderer = GetScene()->GetRenderer();

	//D3D11_MAPPED_SUBRESOURCE msr;
	//renderer->GetContext()->Map(mVertexBuffer->GetVertexBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	////renderer->GetContext()->Map(mVertexBuffer->GetVertexBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	//PlaneVertex* vertex = static_cast<PlaneVertex*>(msr.pData);

	//for (int i = 0; i < mCount; i++)
	//{
	//	vertex[i].tc0 = dx::XMFLOAT2{ vertex[i].tc0.x - 0.001f * deltaTime, vertex[i].tc0.y + 0.002f * deltaTime };
	//	vertex[i].tc1 = dx::XMFLOAT2{ vertex[i].tc1.x + 0.004f * deltaTime, vertex[i].tc1.y - 0.002f * deltaTime };
	//}

	//renderer->GetContext()->Unmap(mVertexBuffer->GetVertexBuffer(), 0);
}

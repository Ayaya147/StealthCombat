#include "PlaneActor.h"
#include "TransformCBuffer.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "MoveComponent.h"
#include "BaseScene.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "DxException.h"

namespace dx = DirectX;

PlaneActor::PlaneActor(BaseScene* scene)
	:
	Actor(scene)
{
	SetTransformCBuffer(new TransformCBuffer(scene->GetSceneManager()->GetRenderer(), this));
	SetScale(0.1f);

	//Mesh* mesh = GetScene()->GetSceneManager()->GetRenderer()->GetMesh("plane", L"Phong");
	Mesh* mesh = GetScene()->GetSceneManager()->GetRenderer()->GetMesh("plane", L"PhongNormalMap",0);
	MeshComponent* mc = new MeshComponent(this, mesh);

	mCount = mesh->GetCount();

	for (auto b : mesh->GetBindables())
	{
		if (auto vertexBuffer = dynamic_cast<VertexBuffer*>(b))
		{
			mVertexBuffer = vertexBuffer;
		}
	}

	//MoveComponent* move = new MoveComponent(this);
	//move->SetAngularSpeed(1.0f);
}

PlaneActor::~PlaneActor()
{
}

void PlaneActor::UpdateActor(float deltaTime)
{
	Renderer* renderer = GetScene()->GetSceneManager()->GetRenderer();

	D3D11_MAPPED_SUBRESOURCE msr;
	renderer->GetContext()->Map(mVertexBuffer->GetVertexBuffer(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &msr);
	//renderer->GetContext()->Map(mVertexBuffer->GetVertexBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	Vertex* vertex = static_cast<Vertex*>(msr.pData);

	for (int i = 0; i < mCount; i++)
	{
		vertex[i].tc0 = dx::XMFLOAT2{ vertex[i].tc0.x - 0.002f * deltaTime, vertex[i].tc0.y + 0.005f * deltaTime };
		vertex[i].tc1 = dx::XMFLOAT2{ vertex[i].tc1.x + 0.008f * deltaTime, vertex[i].tc1.y - 0.005f * deltaTime };
	}

	renderer->GetContext()->Unmap(mVertexBuffer->GetVertexBuffer(), 0);
	//mVertexBuffer->Bind(renderer);
}

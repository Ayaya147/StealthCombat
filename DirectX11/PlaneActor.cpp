#include "PlaneActor.h"
#include "TransformCBufferDouble.h"
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
	mBuffer = new TransformCBuffer(scene->GetSceneManager()->GetRenderer(), this, 0);
	SetScale(1.0f);

	//Mesh* mesh = GetScene()->GetSceneManager()->GetRenderer()->GetMesh("plane", L"Phong");
	Mesh* mesh = GetScene()->GetSceneManager()->GetRenderer()->GetMesh("plane", L"PhongNormalMap");
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
	delete mBuffer;
}

void PlaneActor::UpdateActor(float deltaTime)
{
	Renderer* renderer = GetScene()->GetSceneManager()->GetRenderer();
	D3D11_MAPPED_SUBRESOURCE msr;
	renderer->GetContext()->Map(mVertexBuffer->GetVertexBuffer(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &msr);

	Vertex* vertex = (Vertex*)(msr.pData);

	for (int i = 0; i < mCount; i++)
	{
		vertex[i].tc = dx::XMFLOAT2{ vertex[i].tc.x - 0.1f * deltaTime, vertex[i].tc.y - 0.1f * deltaTime };
	}

	renderer->GetContext()->Unmap(mVertexBuffer->GetVertexBuffer(), 0);
}

void PlaneActor::Bind(Renderer* renderer)
{
	mBuffer->Bind(renderer);
}

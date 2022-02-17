#include "MeshComponent.h"
#include "Actor.h"
#include "BaseScene.h"
#include "Renderer.h"
#include "SceneManager.h"
#include "Mesh.h"
#include "DxException.h"

MeshComponent::MeshComponent(Actor* owner, Mesh* mesh)
	:
	Component(owner),
	mMesh(mesh)
{
	GetOwner()->GetScene()->GetRenderer()->AddMeshComp(mMesh->GetFileName(), this);
}

MeshComponent::~MeshComponent()
{
	GetOwner()->GetScene()->GetRenderer()->RemoveMeshComp(this);
}

void MeshComponent::Draw(Renderer* renderer)
{
	GetOwner()->Bind(renderer);
	renderer->GetContext()->DrawIndexed(mMesh->GetIndicesNum(), 0, 0);
}

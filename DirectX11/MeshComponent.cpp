#include "MeshComponent.h"
#include "Actor.h"
#include "BaseScene.h"
#include "Renderer.h"
#include "GameApp.h"
#include "SceneManager.h"
#include <wrl.h>
#include "Mesh.h"
#include "DxException.h"

namespace wrl = Microsoft::WRL;
namespace dx = DirectX;

MeshComponent::MeshComponent(Actor* owner, Mesh* mesh)
	:
	Component(owner),
	mMesh(mesh)
{
	mOwner->GetScene()->GetSceneManager()->GetApp()->GetRenderer()->AddMeshComp(mMesh->GetFileName(), this);
}

MeshComponent::~MeshComponent()
{
	mOwner->GetScene()->GetSceneManager()->GetApp()->GetRenderer()->RemoveMeshComp(this);
}

void MeshComponent::Draw(Renderer* renderer)
{
	renderer->GetContext()->DrawIndexed(mMesh->GetIndicesNum(), 0, 0);
}

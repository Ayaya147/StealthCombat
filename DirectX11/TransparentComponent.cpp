#include "TransparentComponent.h"
#include "Actor.h"
#include "Mesh.h"
#include "Renderer.h"
#include "BaseScene.h"
#include "CloudActor.h"

TransparentComponent::TransparentComponent(Actor* owner, Mesh* mesh, int drawOrder)
	:
	Component(owner),
	mMesh(mesh),
	mDrawOrder(drawOrder)
{
	GetOwner()->GetScene()->GetRenderer()->AddTranspComp(this);
}

TransparentComponent::~TransparentComponent()
{
	GetOwner()->GetScene()->GetRenderer()->RemoveTranspComp(this);
}

void TransparentComponent::Draw(Renderer* renderer)
{
	GetOwner()->Bind(renderer);
	renderer->GetContext()->DrawIndexed(mMesh->GetIndicesNum(), 0, 0);
}

#include "TranslucenceComponent.h"
#include "Actor.h"
#include "Mesh.h"
#include "Renderer.h"
#include "BaseScene.h"
#include "CloudActor.h"

TranslucenceComponent::TranslucenceComponent(Actor* owner, Mesh* mesh, int drawOrder)
	:
	Component(owner),
	mMesh(mesh),
	mDrawOrder(drawOrder)
{
	GetOwner()->GetScene()->GetRenderer()->AddTranslucenceComp(this);
}

TranslucenceComponent::~TranslucenceComponent()
{
	GetOwner()->GetScene()->GetRenderer()->RemoveTranslucenceComp(this);
}

void TranslucenceComponent::Draw(Renderer* renderer)
{
	GetOwner()->Bind(renderer);
	renderer->GetContext()->DrawIndexed(mMesh->GetIndicesNum(), 0, 0);
}

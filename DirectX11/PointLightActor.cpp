#include "PointLightActor.h"
#include "MeshComponent.h"
#include "Mesh.h"
#include "BaseScene.h"
#include "SceneManager.h"
#include "GameApp.h"
#include "Renderer.h"
#include "ConstantBuffer.h"
#include "TransformCBuffer.h"

PointLightActor::PointLightActor(BaseScene* scene, const std::string& fileName)
	:
	Actor(scene),
	mBuffer1(nullptr),
	mBuffer2(nullptr)
{
	SetTransformCBuffer(new TransformCBuffer(scene->GetSceneManager()->GetApp()->GetRenderer(), this));

	Mesh* mesh = GetScene()->GetSceneManager()->GetApp()->GetRenderer()->GetMesh(fileName, L"Basic");
	MeshComponent* mc = new MeshComponent(this, mesh);

	SetScale(1.0f);
	SetPosition({ 0.0f,0.0f,10.0f });
	PointLightCBuf data =
	{
		GetPosition(),
		{ 0.05f,0.05f,0.05f },
		{ 1.0f,1.0f,1.0f },
		1.0f,
		1.0f,
		0.045f,
		0.0075f,
	};
	mBuffer1 = new PixelConstantBuffer<PointLightCBuf>(scene->GetSceneManager()->GetApp()->GetRenderer(), data, 1);
	mBuffer2 = new PixelConstantBuffer<PSColorConstant>(scene->GetSceneManager()->GetApp()->GetRenderer(), colorConst);
}

PointLightActor::~PointLightActor()
{
	delete mBuffer1;
	delete mBuffer2;
}

void PointLightActor::Bind(Renderer* renderer)
{
	Actor::Bind(renderer);
	mBuffer2->Bind(renderer);
}

void PointLightActor::BindPointLight(Renderer* renderer)
{
	mBuffer1->Bind(renderer);
}

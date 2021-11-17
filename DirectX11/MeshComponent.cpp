#include "MeshComponent.h"
#include "Actor.h"
#include "AbstractScene.h"
#include "GameScene.h"
#include "Renderer.h"
#include "GameApp.h"
#include "SceneManager.h"
#include <wrl.h>
#include "Mesh.h"
#include "DxException.h"

namespace wrl = Microsoft::WRL;
namespace dx = DirectX;

MeshComponent::MeshComponent(class Actor* owner)
	:
	Component(owner)
{
	mOwner->GetGame()->GetSceneManager()->GetApp()->GetRenderer()->AddMeshComp(this);
}

MeshComponent::~MeshComponent()
{
}

void MeshComponent::Draw(Renderer* renderer)
{
	float mAngle = mOwner->GetRotation().x;
	DirectX::XMMATRIX model = mOwner->GetWorldTransform();

	struct ConstantBuffer
	{
		DirectX::XMMATRIX modelView;
		DirectX::XMMATRIX modelViewProj;
	};

	const auto pos = dx::XMVector3Transform(
		dx::XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f),
		dx::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f)
	);

	DirectX::XMMATRIX camera = dx::XMMatrixLookAtLH(pos, dx::XMVectorZero(), dx::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
	) * dx::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);

	const ConstantBuffer cb =
	{
		dx::XMMatrixTranspose(
			model *
			camera
		),

		dx::XMMatrixTranspose(
			model *
			camera *
			dx::XMMatrixPerspectiveLH(1,9.0f / 16.0f,0.5f,100.0f)
		),
	};
	wrl::ComPtr<ID3D11Buffer> pConstantBuffer3;
	D3D11_BUFFER_DESC cbd3;
	cbd3.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd3.Usage = D3D11_USAGE_DYNAMIC;
	cbd3.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd3.MiscFlags = 0u;
	cbd3.ByteWidth = sizeof(cb);
	cbd3.StructureByteStride = 0u;
	D3D11_SUBRESOURCE_DATA csd3 = {};
	csd3.pSysMem = &cb;
	ThrowIfFailed(renderer->GetDevice()->CreateBuffer(&cbd3, &csd3, &pConstantBuffer3));

	renderer->GetContext()->VSSetConstantBuffers(0u, 1u, pConstantBuffer3.GetAddressOf());
	renderer->GetContext()->DrawIndexed((UINT)mMesh->GetIndicesNum(), 0u, 0u);

}

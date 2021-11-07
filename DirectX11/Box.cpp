#include "Box.h"
#include <wrl.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include "DxException.h"
#include "Renderer.h"

namespace wrl = Microsoft::WRL;
namespace dx = DirectX;


Box::Box(Renderer* renderer)
	:
	mRenderer(renderer)
{
	struct  Vertex
	{
		struct
		{
			float x;
			float y;
			float z;
		}pos;
	};

	Vertex vertices[] =
	{
		{ -1.0f,-1.0f,-1.0f	 },
		{ 1.0f,-1.0f,-1.0f	 },
		{ -1.0f,1.0f,-1.0f	 },
		{ 1.0f,1.0f,-1.0f	 },
		{ -1.0f,-1.0f,1.0f	 },
		{ 1.0f,-1.0f,1.0f	 },
		{ -1.0f,1.0f,1.0f	 },
		{ 1.0f,1.0f,1.0f	 },
	};

	wrl::ComPtr<ID3D11Buffer> vertexBuffer;
	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.ByteWidth = sizeof(vertices);
	bd.StructureByteStride = sizeof(Vertex);

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vertices;
	ThrowIfFailed(mRenderer->GetDevice()->CreateBuffer(&bd, &sd, &vertexBuffer));

	const UINT stride = sizeof(Vertex);
	const UINT offset = 0;
	mRenderer->GetContext()->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

	const unsigned short indices[] =
	{
		0,2,1, 2,3,1,
		1,3,5, 3,7,5,
		2,6,3, 3,6,7,
		4,5,7, 4,7,6,
		0,4,2, 2,4,6,
		0,1,4, 1,5,4
	};

	wrl::ComPtr<ID3D11Buffer> indexBuffer;
	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.ByteWidth = sizeof(indices);
	ibd.StructureByteStride = sizeof(unsigned short);
	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = indices;
	ThrowIfFailed(mRenderer->GetDevice()->CreateBuffer(&ibd, &isd, &indexBuffer));

	mRenderer->GetContext()->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);

	struct ConstantBuffer2
	{
		struct
		{
			float r;
			float g;
			float b;
			float a;
		} face_colors[6];
	};
	const ConstantBuffer2 cb2 =
	{
		{
			{1.0f,0.0f,1.0f},
			{1.0f,0.0f,0.0f},
			{0.0f,1.0f,0.0f},
			{0.0f,0.0f,1.0f},
			{1.0f,1.0f,0.0f},
			{0.0f,1.0f,1.0f},
		}
	};
	wrl::ComPtr<ID3D11Buffer> pConstantBuffer2;
	D3D11_BUFFER_DESC cbd2;
	cbd2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd2.Usage = D3D11_USAGE_DEFAULT;
	cbd2.CPUAccessFlags = 0u;
	cbd2.MiscFlags = 0u;
	cbd2.ByteWidth = sizeof(cb2);
	cbd2.StructureByteStride = 0u;
	D3D11_SUBRESOURCE_DATA csd2 = {};
	csd2.pSysMem = &cb2;
	ThrowIfFailed(mRenderer->GetDevice()->CreateBuffer(&cbd2, &csd2, &pConstantBuffer2));

	// bind constant buffer to pixel shader
	mRenderer->GetContext()->PSSetConstantBuffers(0u, 1u, pConstantBuffer2.GetAddressOf());



	// create pixel shader
	wrl::ComPtr<ID3D11PixelShader> pPixelShader;
	wrl::ComPtr<ID3DBlob> pBlob;
	ThrowIfFailed(D3DReadFileToBlob(L"ShaderBins/PixelShader.cso", &pBlob));
	ThrowIfFailed(mRenderer->GetDevice()->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader));

	// bind pixel shader
	mRenderer->GetContext()->PSSetShader(pPixelShader.Get(), nullptr, 0u);


	// create vertex shader
	wrl::ComPtr<ID3D11VertexShader> pVertexShader;
	ThrowIfFailed(D3DReadFileToBlob(L"ShaderBins/VertexShader.cso", &pBlob));
	ThrowIfFailed(mRenderer->GetDevice()->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader));

	// bind vertex shader
	mRenderer->GetContext()->VSSetShader(pVertexShader.Get(), nullptr, 0u);


	// input (vertex) layout (2d position only)
	wrl::ComPtr<ID3D11InputLayout> pInputLayout;
	const D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};
	ThrowIfFailed(mRenderer->GetDevice()->CreateInputLayout(
		ied, (UINT)std::size(ied),
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		&pInputLayout
	));

	mRenderer->GetContext()->IASetInputLayout(pInputLayout.Get());

	mRenderer->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Box::Draw()
{
	mAngle += 0.02f;

	const unsigned short indices[] =
	{
		0,2,1, 2,3,1,
		1,3,5, 3,7,5,
		2,6,3, 3,6,7,
		4,5,7, 4,7,6,
		0,4,2, 2,4,6,
		0,1,4, 1,5,4
	};

	struct ConstantBuffer
	{
		dx::XMMATRIX transform;
	};
	const ConstantBuffer cb =
	{
		{
			dx::XMMatrixTranspose(
				dx::XMMatrixRotationZ(mAngle) *
				dx::XMMatrixRotationX(mAngle) *
				dx::XMMatrixTranslation(0.0f,0.0f,5.0f) *
				dx::XMMatrixPerspectiveLH(1,9.0f/16.0f,0.5f,10.0f)
			)
		}
	};
	wrl::ComPtr<ID3D11Buffer> pConstantBuffer;
	D3D11_BUFFER_DESC cbd;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0u;
	cbd.ByteWidth = sizeof(cb);
	cbd.StructureByteStride = 0u;
	D3D11_SUBRESOURCE_DATA csd = {};
	csd.pSysMem = &cb;
	ThrowIfFailed(mRenderer->GetDevice()->CreateBuffer(&cbd, &csd, &pConstantBuffer));

	mRenderer->GetContext()->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());

	mRenderer->GetContext()->DrawIndexed((UINT)std::size(indices), 0u, 0u);
}

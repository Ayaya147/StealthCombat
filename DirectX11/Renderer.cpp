#include "Renderer.h"
#include "DxException.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "Texture.h"
#include "Actor.h"
#include "BaseScene.h"
#include "PointLightActor.h"
#include "LightCBuffer.h"

namespace dx = DirectX;
namespace wrl = Microsoft::WRL;

Renderer::Renderer(HWND hWnd, int width, int height)
	:
	mProjection(dx::XMMatrixPerspectiveLH(1, 9.0f / 16.0f, 0.5f, 100.0f))
{
	UINT createDeviceFlags = 0;
#ifdef DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	ThrowIfFailed(D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		0,
		createDeviceFlags,
		nullptr, 0,
		D3D11_SDK_VERSION,
		&mDevice,
		nullptr,
		&mContext
	));

	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hWnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	wrl::ComPtr<IDXGIDevice> dxgiDevice;
	ThrowIfFailed(mDevice->QueryInterface(__uuidof(IDXGIDevice), &dxgiDevice));
	wrl::ComPtr<IDXGIAdapter> dxgiAdapter;
	ThrowIfFailed(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), &dxgiAdapter));
	wrl::ComPtr<IDXGIFactory> dxgiFactory;
	ThrowIfFailed(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), &dxgiFactory));
	ThrowIfFailed(dxgiFactory->CreateSwapChain(mDevice.Get(), &sd, &mSwapChain));

	wrl::ComPtr<ID3D11Texture2D> backBuffer;
	ThrowIfFailed(mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &backBuffer));
	ThrowIfFailed(mDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, &mRenderTargetView));

	wrl::ComPtr<ID3D11Texture2D> depthStencilBuffer;
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = width;
	depthStencilDesc.Height = height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	ThrowIfFailed(mDevice->CreateTexture2D(&depthStencilDesc, nullptr, &depthStencilBuffer));
	ThrowIfFailed(mDevice->CreateDepthStencilView(depthStencilBuffer.Get(), nullptr, &mDepthStencilView));
	mContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), mDepthStencilView.Get());

	D3D11_VIEWPORT vp;
	vp.Width = static_cast<FLOAT>(width);
	vp.Height = static_cast<FLOAT>(height);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	mContext->RSSetViewports(1u, &vp);

	mLightCBuffer = new LightCBuffer(this);
}

Renderer::~Renderer()
{
}

void Renderer::Draw()
{
	const float color[] = { 0.2f,0.2f,0.2f,1.0f };
	mContext->ClearRenderTargetView(mRenderTargetView.Get(), color);
	mContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

	mLightCBuffer->Bind(this);
	std::string name;
	for (auto mc : mMeshComps)
	{
		for (auto m : mc.second)
		{
			if (name != mc.first)
			{
				m->GetMesh()->Bind(this);
				name = mc.first;
			}
			m->GetOwner()->Bind(this);
			m->Draw(this);
		}
	}

	mSwapChain->Present(1, 0);
}

void Renderer::UnloadData()
{
	for (auto i : mTextures)
	{
		delete i.second;
	}
	mTextures.clear();

	for (auto i : mMeshes)
	{
		delete i.second;
	}
	mMeshes.clear();

	delete mLightCBuffer;
}

void Renderer::AddMeshComp(const std::string& name, MeshComponent* mesh)
{
	mMeshComps[name].emplace_back(mesh);
}

void Renderer::RemoveMeshComp(MeshComponent* mesh)
{
	for (auto mc : mMeshComps)
	{
		auto iter = std::find(mc.second.begin(), mc.second.end(), mesh);
		if (iter != mc.second.end())
		{
			mc.second.erase(iter);
			break;
		}
	}
}

Mesh* Renderer::GetMesh(const std::string& fileName, const std::wstring& shaderName)
{
	Mesh* mesh = nullptr;
	auto iter = mMeshes.find(fileName);
	if (iter != mMeshes.end())
	{
		mesh = iter->second;
	}
	else
	{
		mesh = new Mesh(this, fileName, shaderName);
		mMeshes.emplace(fileName, mesh);
	}
	return mesh;
}

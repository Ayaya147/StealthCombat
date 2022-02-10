#include <dxgi.h>
#include <algorithm>
#include "Renderer.h"
#include "Texture.h"
#include "Stencil.h"
#include "VertexBuffer.h"
#include "Topology.h"
#include "InputLayout.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Blender.h"
#include "Sampler.h"
#include "DxException.h"
#include "PlaneMesh.h"
#include "MeshComponent.h"
#include "TransparentComponent.h"
#include "SpriteComponent.h"
#include "Texture.h"
#include "Actor.h"
#include "CloudActor.h"
#include "PlaneActor.h"
#include "BaseScene.h"
#include "GameScene.h"
#include "Light.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui//imgui_impl_win32.h"

namespace dx = DirectX;
namespace wrl = Microsoft::WRL;

Renderer::Renderer(HWND hWnd, int width, int height)
	:
	mProjection(dx::XMMatrixPerspectiveLH(1, static_cast<float>(height) / static_cast<float>(width), 0.5f, 500.0f)),
	mProjection2D(dx::XMMatrixOrthographicOffCenterLH(-960.0f, 960.0f, 540.0f, -540.0f, 0.0f, 1.0f))
{
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hWnd;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	wrl::ComPtr<IDXGIFactory1> dxgiFactory;
	CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));
	std::vector<wrl::ComPtr<IDXGIAdapter>> adapters;
	wrl::ComPtr<IDXGIAdapter> tmpAdapter;

	for (int i = 0; dxgiFactory->EnumAdapters(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND; i++)
	{
		adapters.emplace_back(tmpAdapter);
	}

	for (auto adpt : adapters)
	{
		DXGI_ADAPTER_DESC adesc = {};
		adpt->GetDesc(&adesc);

		std::wstring strDesc = adesc.Description;
		if (strDesc.find(L"NVIDIA") != std::string::npos || strDesc.find(L"AMD") != std::string::npos)
		{
			tmpAdapter = adpt;
			break;
		}
	}

	D3D_DRIVER_TYPE driverType;
	if (tmpAdapter)
	{
		driverType = D3D_DRIVER_TYPE_UNKNOWN;
	}
	else
	{
		driverType = D3D_DRIVER_TYPE_HARDWARE;
	}

	UINT createDeviceFlags = 0;
#ifdef DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	ThrowIfFailed(D3D11CreateDeviceAndSwapChain(
		tmpAdapter.Get(),
		driverType,
		nullptr,
		createDeviceFlags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&mSwapChain,
		&mDevice,
		nullptr,
		&mContext
	));

	wrl::ComPtr<ID3D11Texture2D> backBuffer;
	ThrowIfFailed(mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &backBuffer));
	ThrowIfFailed(mDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, &mRenderTargetView));

	wrl::ComPtr<ID3D11Texture2D> depthStencilBuffer;
	D3D11_TEXTURE2D_DESC depthStencilDesc = {};
	depthStencilDesc.Width = width;
	depthStencilDesc.Height = height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	ThrowIfFailed(mDevice->CreateTexture2D(&depthStencilDesc, nullptr, &depthStencilBuffer));

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;
	ThrowIfFailed(mDevice->CreateDepthStencilView(depthStencilBuffer.Get(), &dsvDesc, &mDepthStencilView));
	mContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), mDepthStencilView.Get());

	D3D11_VIEWPORT vp = {};
	vp.Width = static_cast<FLOAT>(width);
	vp.Height = static_cast<FLOAT>(height);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	mContext->RSSetViewports(1, &vp);

	mDepthStencilOff = new Stencil(this, Stencil::Mode::EOff);
	mDepthStencilOn = new Stencil(this, Stencil::Mode::EOn);
	mSampler = new Sampler(this);
	mBlenderOn = new Blender(this, true);
	mBlenderOff = new Blender(this, false);
	mLight = new Light(this);

	Create2DBuffer();

	ImGui_ImplDX11_Init(mDevice.Get(), mContext.Get());
}

Renderer::~Renderer()
{
	delete mLight;
	delete mDepthStencilOff;
	delete mDepthStencilOn;
	delete mVertexBuffer;
	delete mVertexShader;
	delete mPixelShader;
	delete mTopology;
	delete mInputLayout;
	delete mSampler;
	delete mBlenderOff;
	delete mBlenderOn;
	ImGui_ImplDX11_Shutdown();
}

void Renderer::Draw()
{
#ifdef DEBUG
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
#endif

	const float color[] = { 0.2f,0.2f,0.2f,1.0f };
	mContext->ClearRenderTargetView(mRenderTargetView.Get(), color);
	mContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

	Draw3DScene();
	Draw2DScene();

#ifdef DEBUG
	if (auto game = dynamic_cast<GameScene*>(mScene))
	{
		game->GetCloud()->ImGuiWindow();
		game->GetPlane()->ImGuiWindow();
		mLight->ImGuiWindow();
	}

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#endif

	mSwapChain->Present(1, 0);
}

void Renderer::Draw3DScene()
{
	mSampler->Bind(this);
	mDepthStencilOn->Bind(this);
	mBlenderOff->Bind(this);
	mLight->Bind(this);

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

	//auto distTest = [](TransparentComponent* tc1, TransparentComponent* tc2)
	//{
	//	return tc1->GetDistFromCamera() > tc2->GetDistFromCamera();
	//};
	//std::sort(mTranspComps.begin(), mTranspComps.end(), distTest);
	mDepthStencilOff->Bind(this);
	mBlenderOn->Bind(this);
	bool isBind = false;
	for (auto tc : mTranspComps)
	{
		if (!isBind)
		{
			tc->GetMesh()->Bind(this);
			isBind = true;
		}
		tc->GetOwner()->Bind(this);
		tc->Draw(this);
	}
}

void Renderer::Draw2DScene()
{
	mVertexBuffer->Bind(this);
	mTopology->Bind(this);
	mInputLayout->Bind(this);
	mVertexShader->Bind(this);
	mPixelShader->Bind(this);

	for (auto sprite : mSpriteComps)
	{
		sprite->Draw(this);
	}
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

	mMeshComps.clear();
}

void Renderer::AddSprite(SpriteComponent* sprite)
{
	int myDrawOrder = sprite->GetDrawOrder();
	auto iter = mSpriteComps.begin();
	for (; iter != mSpriteComps.end(); ++iter)
	{
		if (myDrawOrder < (*iter)->GetDrawOrder())
		{
			break;
		}
	}

	mSpriteComps.insert(iter, sprite);
}

void Renderer::RemoveSprite(SpriteComponent* sprite)
{
	auto iter = std::find(mSpriteComps.begin(), mSpriteComps.end(), sprite);
	mSpriteComps.erase(iter);
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

void Renderer::AddTranspComp(TransparentComponent* transparent)
{
	int myDrawOrder = transparent->GetDrawOrder();
	auto iter = mTranspComps.begin();
	for (; iter != mTranspComps.end(); ++iter)
	{
		if (myDrawOrder < (*iter)->GetDrawOrder())
		{
			break;
		}
	}

	mTranspComps.insert(iter, transparent);
}

void Renderer::RemoveTranspComp(TransparentComponent* transparent)
{
	auto iter = std::find(mTranspComps.begin(), mTranspComps.end(), transparent);
	mTranspComps.erase(iter);
}

void Renderer::Create2DBuffer()
{
	struct Vertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 tc;
	};

	std::vector<Vertex> vertices;
	vertices.reserve(4);
	vertices.push_back({ {-0.5f,-0.5f,0.0f} ,{0.0f,0.0f} });
	vertices.push_back({ { 0.5f,-0.5f,0.0f} ,{1.0f,0.0f} });
	vertices.push_back({ {-0.5f, 0.5f,0.0f} ,{0.0f,1.0f} });
	vertices.push_back({ { 0.5f, 0.5f,0.0f} ,{1.0f,1.0f} });

	const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TexCoord",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};

	mVertexBuffer = new VertexBuffer(this, vertices);
	mVertexShader = new VertexShader(this, L"ShaderBins\\SpriteVS.cso");
	mPixelShader = new PixelShader(this, L"ShaderBins\\SpritePS.cso");
	mTopology = new Topology(this, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	mInputLayout = new InputLayout(this, ied, mVertexShader);
}

Mesh* Renderer::GetMesh(const std::string& fileName)
{
	Mesh* mesh = nullptr;
	auto iter = mMeshes.find(fileName);
	if (iter != mMeshes.end())
	{
		mesh = iter->second;
	}
	else
	{
		if (fileName != "plane")
		{
			mesh = new Mesh(fileName);
		}
		else
		{
			mesh = new PlaneMesh(fileName);
		}
		mMeshes.emplace(fileName, mesh);
	}
	return mesh;
}

Texture* Renderer::GetTexture(const std::string& fileName)
{
	Texture* tex = nullptr;
	auto iter = mTextures.find(fileName);
	if (iter != mTextures.end())
	{
		tex = iter->second;
	}
	else
	{
		tex = new Texture(this, fileName);
		mTextures.emplace(fileName, tex);
	}
	return tex;
}

void Renderer::SetDirectionalLight(const DirectionalLight& light)
{
	mLight->SetDirectionalLight(light);
}

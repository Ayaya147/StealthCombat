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
#include "TranslucenceComponent.h"
#include "SpriteComponent.h"
#include "Texture.h"
#include "Actor.h"
#include "CloudActor.h"
#include "PlaneActor.h"
#include "ExplosionActor.h"
#include "SmokeActor.h"
#include "EmissionActor.h"
#include "DemoScene.h"
#include "Fade.h"
#include "Light.h"
#include "PauseScreen.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui//imgui_impl_win32.h"

namespace dx = DirectX;
namespace wrl = Microsoft::WRL;

Renderer::Renderer(HWND hWnd, int width, int height)
{
	InitDirectX(hWnd, width, height);
	CreateBuffer();
	CreateProjectionMatrix(width, height);
	ImGui_ImplDX11_Init(mDevice.Get(), mContext.Get());

	mLight = new Light(this);
}

Renderer::~Renderer()
{
	delete mLight;
	delete mDepthOff;
	delete mDepthOn;
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
	auto demo = dynamic_cast<DemoScene*>(mScene);
	bool isDemo = demo && mScene->GetFade()->GetFadeState() == Fade::FadeState::EFadeNone;

	// imgui begin frame
	if (isDemo)
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

	const float color[] = { 0.2f,0.2f,0.2f,1.0f };
	mContext->ClearRenderTargetView(mRenderTargetView.Get(), color);
	mContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

	Draw3DScene();
	Draw2DScene();

	// imgui windows
	if (isDemo)
	{
		demo->GetCloud()->ImGuiWindow();
		demo->GetPlane()->ImGuiWindow();
		demo->GetExplosion()->ImGuiWindow();
		demo->GetSmoke()->ImGuiWindow();
		demo->GetEmission()->ImGuiWindow();
		mLight->ImGuiWindow();

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	mSwapChain->Present(1, 0);
}

void Renderer::Draw3DScene()
{
	mSampler->Bind(this);
	mDepthOn->Bind(this);
	mBlenderOff->Bind(this);
	mLight->Bind(this);

	// draw opaque 3D objects
	Mesh* lastMesh = nullptr;
	for (auto mc : mMeshComps)
	{
		for (auto m : mc.second)
		{
			Mesh* mesh = m->GetMesh();
			if (mesh != lastMesh)
			{
				mesh->Bind(this);
				lastMesh = mesh;
			}
			m->Draw(this);
		}
	}

	// draw translucent 3D objects
	mDepthOff->Bind(this);
	mBlenderOn->Bind(this);
	bool isBind = false;
	for (auto tc : mTranslucenceComps)
	{
		if (!isBind)
		{
			tc->GetMesh()->Bind(this);
			isBind = true;
		}
		tc->Draw(this);
	}
}

void Renderer::Draw2DScene()
{
	mTopology->Bind(this);
	mInputLayout->Bind(this);
	mVertexShader->Bind(this);
	mPixelShader->Bind(this);

	// draw 2D sprites
	Texture* lastTex = nullptr;
	for (auto sprite : mSpriteComps)
	{
		if (sprite->GetIsVisible())
		{
			Texture* tex = sprite->GetTexture();
			if (tex != lastTex)
			{
				tex->Bind(this);
				lastTex = tex;
			}
			sprite->Draw(this, mVertexBuffer);
		}
	}

	// draw 2D pause screen
	for (auto ui : mScene->GetPauseUIStack())
	{
		ui->Draw(this, mVertexBuffer);
	}

	// draw fade
	mScene->GetFade()->Draw(this, mVertexBuffer);
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
	for (auto& mc : mMeshComps)
	{
		auto iter = std::find(mc.second.begin(), mc.second.end(), mesh);
		if (iter != mc.second.end())
		{
			mc.second.erase(iter);
			break;
		}
	}
}

void Renderer::AddTranslucenceComp(TranslucenceComponent* translucence)
{
	int myDrawOrder = translucence->GetDrawOrder();
	auto iter = mTranslucenceComps.begin();
	for (; iter != mTranslucenceComps.end(); ++iter)
	{
		if (myDrawOrder < (*iter)->GetDrawOrder())
		{
			break;
		}
	}

	mTranslucenceComps.insert(iter, translucence);
}

void Renderer::RemoveTranslucenceComp(TranslucenceComponent* translucence)
{
	auto iter = std::find(mTranslucenceComps.begin(), mTranslucenceComps.end(), translucence);
	mTranslucenceComps.erase(iter);
}

void Renderer::ResetLight()
{
	mLight->Reset();
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
	std::string name = "Asset\\Texture\\" + fileName + ".png";
	Texture* tex = nullptr;

	auto iter = mTextures.find(name);
	if (iter != mTextures.end())
	{
		tex = iter->second;
	}
	else
	{
		tex = new Texture(this, name);
		mTextures.emplace(name, tex);
	}

	return tex;
}

void Renderer::SetDirectionalLight(const DirectionalLight& light)
{
	mLight->SetDirectionalLight(light);
}

void Renderer::InitDirectX(HWND hWnd, int width, int height)
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

	UINT createDeviceFlags = 0;
#ifdef DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// search for adapters
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

	// create device, swap chain and rendering context
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

	// gain access to back buffer
	wrl::ComPtr<ID3D11Texture2D> backBuffer;
	ThrowIfFailed(mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &backBuffer));
	ThrowIfFailed(mDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, &mRenderTargetView));

	// create depth stencil texture
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

	// viewport
	D3D11_VIEWPORT vp = {};
	vp.Width = static_cast<FLOAT>(width);
	vp.Height = static_cast<FLOAT>(height);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	mContext->RSSetViewports(1, &vp);
}

void Renderer::CreateBuffer()
{
	mDepthOff = new Stencil(this, Stencil::Mode::EOff);
	mDepthOn = new Stencil(this, Stencil::Mode::EOn);
	mSampler = new Sampler(this);
	mBlenderOff = new Blender(this, Blender::Mode::EOff);
	mBlenderOn = new Blender(this, Blender::Mode::EOn);

	std::vector<Vertex> vertices;
	vertices.reserve(4);
	vertices.push_back({ {-0.5f,-0.5f,0.0f},{1.0f,1.0f,1.0f,1.0f},{0.0f,0.0f} });
	vertices.push_back({ { 0.5f,-0.5f,0.0f},{1.0f,1.0f,1.0f,1.0f},{1.0f,0.0f} });
	vertices.push_back({ {-0.5f, 0.5f,0.0f},{1.0f,1.0f,1.0f,1.0f},{0.0f,1.0f} });
	vertices.push_back({ { 0.5f, 0.5f,0.0f},{1.0f,1.0f,1.0f,1.0f},{1.0f,1.0f} });

	const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,28,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};

	mVertexBuffer = new VertexBuffer(this, vertices);
	mVertexShader = new VertexShader(this, L"ShaderBin\\SpriteVS.cso");
	mPixelShader = new PixelShader(this, L"ShaderBin\\SpritePS.cso");
	mTopology = new Topology(this, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	mInputLayout = new InputLayout(this, ied, mVertexShader);
}

void Renderer::CreateProjectionMatrix(int width, int height)
{
	dx::XMMATRIX projection = dx::XMMatrixPerspectiveLH(
		1, static_cast<float>(height) / static_cast<float>(width),
		0.5f, 500.0f
	);
	dx::XMStoreFloat4x4(&mProjection, projection);

	dx::XMMATRIX projection2D = dx::XMMatrixOrthographicOffCenterLH(
		-960.0f, 960.0f, 540.0f, -540.0f, 0.0f, 1.0f
	);
	dx::XMStoreFloat4x4(&mProjection2D, projection2D);
}

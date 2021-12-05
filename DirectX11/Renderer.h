#pragma once
#include <wrl.h>
#include <d3d11.h>
#include <string>
#include <unordered_map>
#include <DirectXMath.h>

class BaseScene;
class Texture;
class Mesh;
class MeshComponent;
class LightCBuffer;

template<class C>
class PixelConstantBuffer;

struct DirectionalLight
{
	alignas(16) DirectX::XMFLOAT3 mDirection;
	alignas(16) DirectX::XMFLOAT3 mDiffuseColor;
	alignas(16) DirectX::XMFLOAT3 mSpecColor;
};

class Renderer
{
public:
	Renderer(HWND hWnd, int width, int height);
	~Renderer();

	void Draw();
	void UnloadData();
	void AddMeshComp(const std::string& name, MeshComponent* mesh);
	void RemoveMeshComp(MeshComponent* mesh);

	Mesh* GetMesh(const std::string& fileName, const std::wstring& shaderName);
	ID3D11Device* GetDevice() const { return mDevice.Get(); }
	ID3D11DeviceContext* GetContext() const { return mContext.Get(); }
	const DirectX::XMMATRIX& GetViewMatrix() const { return mView; }
	const DirectX::XMMATRIX& GetProjectionMatrix() const { return mProjection; }
	const DirectX::XMFLOAT3& GetAmbientLight() const { return mAmbientLight; }
	const DirectionalLight& GetDirectionalLight() const { return mDirLight; }

	void SetViewMatrix(const DirectX::XMMATRIX& view) { mView = view; }
	void SetScene(BaseScene* scene) { mScene = scene; }
	void SetAmbientLight(const DirectX::XMFLOAT3& ambient) { mAmbientLight = ambient; }
	void SetDirectionalLight(const DirectionalLight& direct) { mDirLight = direct; }

private:
	Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> mContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> mDepthStencilView;

	std::unordered_map<std::string, Mesh*> mMeshes;
	std::unordered_map<std::string, Texture*> mTextures;
	std::unordered_map<std::string, std::vector<MeshComponent*>> mMeshComps;

	DirectX::XMMATRIX mView;
	DirectX::XMMATRIX mProjection;

	DirectX::XMFLOAT3 mAmbientLight;
	DirectionalLight mDirLight;
	LightCBuffer* mLightCBuffer;

	BaseScene* mScene;
};

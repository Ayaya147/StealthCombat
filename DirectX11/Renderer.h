#pragma once
#include <wrl.h>
#include <d3d11.h>
#include <string>
#include <unordered_map>
#include <DirectXMath.h>

template<class C>
class PixelConstantBuffer;

class Renderer
{
public:
	Renderer(HWND hWnd, int width, int height);
	~Renderer();

	void Draw();
	void UnloadData();
	void AddMeshComp(const std::string& name, class MeshComponent* mesh);
	void RemoveMeshComp(class MeshComponent* mesh);

	class Mesh* GetMesh(const std::string& fileName, const std::wstring& shaderName, int test);
	ID3D11Device* GetDevice() const { return mDevice.Get(); }
	ID3D11DeviceContext* GetContext() const { return mContext.Get(); }
	const DirectX::XMMATRIX& GetViewMatrix() const { return mView; }
	const DirectX::XMMATRIX& GetProjectionMatrix() const { return mProjection; }
	//const DirectX::XMFLOAT3& GetAmbientLight() const { return mAmbientLight; }
	//const DirectionalLight& GetDirectionalLight() const { return mDirLight; }

	void SetViewMatrix(const DirectX::XMMATRIX& view) { mView = view; }
	void SetScene(class BaseScene* scene) { mScene = scene; }
	//void SetAmbientLight(const DirectX::XMFLOAT3& ambient);
	//void SetDirectionalLight(const class DirectionalLight& direct);

private:
	Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> mContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> mDepthStencilView;

	std::unordered_map<std::string, class Mesh*> mMeshes;
	std::unordered_map<std::string, class Texture*> mTextures;
	std::unordered_map<std::string, std::vector<class MeshComponent*>> mMeshComps;

	DirectX::XMMATRIX mView;
	DirectX::XMMATRIX mProjection;

	//DirectX::XMFLOAT3 mAmbientLight;
	//DirectionalLight mDirLight;
	class Light* mLight;

	class BaseScene* mScene;
};

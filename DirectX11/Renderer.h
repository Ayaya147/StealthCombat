#pragma once
#include <wrl.h>
#include <d3d11.h>
#include <string>
#include <unordered_map>

class Renderer
{
public:
	Renderer(HWND hWnd, int width, int height);
	~Renderer();

	void Draw();
	void UnloadData();
	void AddMeshComp(class MeshComponent* mesh);
	void RemoveMeshComp(class MeshComponent* mesh);

	class Mesh* GetMesh(const std::string& fileName);
	ID3D11Device* GetDevice() const { return mDevice.Get(); }
	ID3D11DeviceContext* GetContext() const { return mContext.Get(); }

private:
	Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> mContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> mDepthStencilView;

	std::unordered_map<std::string, class Mesh*> mMeshes;
	std::unordered_map<std::string, class Texture*> mTextures;
	std::vector<class MeshComponent*> mMeshComps;
};

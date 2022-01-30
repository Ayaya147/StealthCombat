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
	void Draw3DScene();
	void Draw2DScene();
	void UnloadData();
	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);
	void AddMeshComp(const std::string& name, class MeshComponent* mesh);
	void RemoveMeshComp(class MeshComponent* mesh);
	void AddTranspComp(class TransparentComponent* mesh);
	void RemoveTranspComp(class TransparentComponent* mesh);

	class Mesh* GetMesh(const std::string& fileName);
	ID3D11Device* GetDevice() const { return mDevice.Get(); }
	ID3D11DeviceContext* GetContext() const { return mContext.Get(); }
	const DirectX::XMMATRIX& GetViewMatrix() const { return mView; }
	const DirectX::XMMATRIX& GetProjectionMatrix() const { return mProjection; }

	void SetViewMatrix(const DirectX::XMMATRIX& view) { mView = view; }
	void SetScene(class BaseScene* scene) { mScene = scene; }
	void SetAmbientLight(const DirectX::XMFLOAT3& ambient);
	void SetDirectionalLight(const DirectX::XMFLOAT3& dir, const DirectX::XMFLOAT3& diff, const DirectX::XMFLOAT3& spec);

private:
	Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> mContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> mDepthStencilView;

	std::unordered_map<std::string, class Mesh*> mMeshes;
	std::unordered_map<std::string, class Texture*> mTextures;
	std::unordered_map<std::string, std::vector<class MeshComponent*>> mMeshComps;
	std::vector<class TransparentComponent*> mTranspComps;
	std::vector<class SpriteComponent*> mSprites;

	DirectX::XMMATRIX mView;
	DirectX::XMMATRIX mProjection;

	class Stencil* mDepthStencilOn;
	class Stencil* mDepthStencilOff;
	class Light* mLight;
	class BaseScene* mScene;
};

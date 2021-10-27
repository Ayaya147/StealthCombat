#pragma once
#include <wrl.h>
#include <d3d11.h>
#include <memory>

class Renderer
{
public:
	Renderer(HWND hWnd, int width, int height);
	~Renderer();

	void Draw();

	Microsoft::WRL::ComPtr<ID3D11Device> GetDevice() { return mDevice.Get(); }
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> GetContext() { return mContext.Get(); }


private:
	Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> mContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> mDepthStencilView;

	std::unique_ptr<class Box> mBox;
};

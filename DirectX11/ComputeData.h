#pragma once
#include <d3d11.h>
#include <wrl.h>

class ComputeData
{
public:
	ComputeData(class Renderer* renderer, int dataSize, int structSize, void* initData = nullptr);
	~ComputeData();

	ID3D11ShaderResourceView* const* GetSRVAddress() const { return mSRV.GetAddressOf(); }
	ID3D11Buffer* GetBuffer() const { return mBuffer.Get(); }
	ID3D11UnorderedAccessView* GetUAV() const { return mUAV.Get(); }
	ID3D11ShaderResourceView* GetSRV() const { return mSRV.Get(); }

private:
	void InitBuf(class Renderer* renderer, int dataSize, int structSize, void* initData);
	void InitUAV(class Renderer* renderer);
	void InitSRV(class Renderer* renderer);	

	Microsoft::WRL::ComPtr<ID3D11Buffer> mBuffer;
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> mUAV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mSRV;
};

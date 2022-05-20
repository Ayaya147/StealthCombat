#pragma once
#include <d3d11.h>
#include <wrl.h>

class ComputeData
{
public:
	ComputeData(class Renderer* renderer, int dataSize, int structSize, void* initData = nullptr);
	~ComputeData();

	Microsoft::WRL::ComPtr<ID3D11Buffer> mBuffer;
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> mUAV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mSRV;

private:
	void InitBuf(class Renderer* renderer, int dataSize, int structSize, void* initData);
	void InitUAV(class Renderer* renderer);
	void InitSRV(class Renderer* renderer);	
};

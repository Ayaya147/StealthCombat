#pragma once
#include <d3d11.h>

class ComputeData
{
public:
	~ComputeData();

	void InitBuf(class Renderer* renderer, int data_size, int structSize, void* initData = nullptr);
	void InitUAV(class Renderer* renderer);
	void InitSRV(class Renderer* renderer);


	ID3D11Buffer* buf = nullptr;
	ID3D11ShaderResourceView* srv = nullptr;
	ID3D11UnorderedAccessView* uav = nullptr;
};

#pragma once
#include "Bindable.h"
#include "Renderer.h"

template<class C>
class PixelConstantBuffer;

class LightCBuffer : public Bindable
{
public:
	LightCBuffer(Renderer* renderer);
	~LightCBuffer();

	void Bind(Renderer* renderer) override;

private:
	struct Constant
	{
		alignas(16) DirectX::XMFLOAT3 cameraPos;
		alignas(16) DirectX::XMFLOAT3 ambientLight;
		float specPower;
		float padding[3];
	};

	PixelConstantBuffer<Constant>* mCBuffer;
	PixelConstantBuffer<DirectionalLight>* mLightBuffer;
};

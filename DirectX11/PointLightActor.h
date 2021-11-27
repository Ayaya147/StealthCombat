#pragma once
#include <string>
#include "Actor.h"

template<class C>
class PixelConstantBuffer;

class PointLightActor : public Actor
{
public:
	PointLightActor(class BaseScene* scene, const std::string& fileName);
	~PointLightActor();

	void Bind(class Renderer* renderer) override;
	void BindPointLight(class Renderer* renderer);

private:
	struct PointLightCBuf
	{
		alignas(16) DirectX::XMFLOAT3 pos;
		alignas(16) DirectX::XMFLOAT3 ambient;
		alignas(16) DirectX::XMFLOAT3 diffuseColor;
		float diffuseIntensity;
		float attConst;
		float attLin;
		float attQuad;
	};

	struct PSColorConstant
	{
		DirectX::XMFLOAT3 color = { 1.0f,1.0f,1.0f };
		float padding;
	}colorConst;

	PixelConstantBuffer<PointLightCBuf>* mBuffer1;
	PixelConstantBuffer<PSColorConstant>* mBuffer2;
};

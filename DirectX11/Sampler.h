#pragma once
#include "Bindable.h"

class Sampler : public Bindable
{
public:
	Sampler(class Renderer* renderer);

	void Bind(class Renderer* renderer) override;

private:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> mSampler;
};

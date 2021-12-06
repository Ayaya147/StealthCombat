#pragma once
#include "Bindable.h"

class Sampler : public Bindable
{
public:
	Sampler(Renderer* renderer);

	void Bind(Renderer* renderer) override;

private:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> mSampler;
};

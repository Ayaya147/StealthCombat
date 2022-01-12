#pragma once
#include <array>
#include <optional>
#include "Bindable.h"

class Blender : public Bindable
{
public:
	Blender(class Renderer* renderer, bool blending, std::optional<float> factors = {});

	void Bind(class Renderer* renderer) override;

private:
	Microsoft::WRL::ComPtr<ID3D11BlendState> mBlender;
	std::optional<std::array<float, 4>> mFactors;
};

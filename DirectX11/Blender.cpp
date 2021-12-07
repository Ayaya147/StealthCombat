#include "Blender.h"
#include "Renderer.h"
#include "DxException.h"

Blender::Blender(Renderer* renderer, bool blending, std::optional<float> factors)
{
	if (factors)
	{
		mFactors.emplace();
		mFactors->fill(*factors);
	}

	D3D11_BLEND_DESC blendDesc = CD3D11_BLEND_DESC{ CD3D11_DEFAULT{} };
	auto& brt = blendDesc.RenderTarget[0];
	if (blending)
	{
		brt.BlendEnable = TRUE;

		if (factors)
		{
			brt.SrcBlend = D3D11_BLEND_BLEND_FACTOR;
			brt.DestBlend = D3D11_BLEND_INV_BLEND_FACTOR;
		}
		else
		{
			brt.SrcBlend = D3D11_BLEND_SRC_ALPHA;
			brt.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		}
	}
	ThrowIfFailed(renderer->GetDevice()->CreateBlendState(&blendDesc, &mBlender));

}

void Blender::Bind(Renderer* renderer)
{
	const float* data = mFactors ? mFactors->data() : nullptr;
	renderer->GetContext()->OMSetBlendState(mBlender.Get(), data, 0xFFFFFFFF);
}

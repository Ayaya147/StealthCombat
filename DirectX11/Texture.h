#pragma once
#include <string>
#include "Bindable.h"
#include "Dxtex/DirectXTex.h"

class Texture : public Bindable
{
public:
	Texture(class Renderer* renderer, const std::string& fileName, UINT slot = 0);
	
	void Bind(class Renderer* renderer) override;
	void LoadTexture(const std::string& fileName);

private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mTextureView;
	DirectX::ScratchImage mScratch;
	UINT mSlot;
};

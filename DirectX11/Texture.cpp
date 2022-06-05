#include "Texture.h"
#include "Renderer.h"
#include "DxException.h"

namespace dx = DirectX;
namespace wrl = Microsoft::WRL;

Texture::Texture(Renderer* renderer, const std::string& fileName, UINT slot)
	:
	mSlot(slot),
	mFileName(fileName)
{
	LoadTexture(fileName);

	mWidth = static_cast<UINT>(mScratch.GetMetadata().width);
	mHeight = static_cast<UINT>(mScratch.GetMetadata().height);

	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = mWidth;
	textureDesc.Height = mHeight;
	textureDesc.MipLevels = 0;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	wrl::ComPtr<ID3D11Texture2D> texture;
	ThrowIfFailed(renderer->GetDevice()->CreateTexture2D(&textureDesc, nullptr, &texture));

	renderer->GetContext()->UpdateSubresource(
		texture.Get(), 0, nullptr, mScratch.GetPixels(),
		static_cast<UINT>(mScratch.GetMetadata().width * sizeof(UINT)), 0
	);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;
	ThrowIfFailed(renderer->GetDevice()->CreateShaderResourceView(
		texture.Get(), &srvDesc, &mTextureView
	));

	renderer->GetContext()->GenerateMips(mTextureView.Get());
}

Texture::Texture(Renderer* renderer, UINT slot)
	:
	mHeight(0),
	mWidth(0),
	mSlot(slot)
{
	mTextureView = renderer->GetScreenshotViewPtr();
}

void Texture::Bind(Renderer* renderer)
{
	renderer->GetContext()->PSSetShaderResources(mSlot, 1, mTextureView.GetAddressOf());
}

void Texture::LoadTexture(const std::string& fileName)
{
	wchar_t wideName[512];
	mbstowcs_s(nullptr, wideName, fileName.c_str(), _TRUNCATE);

	dx::ScratchImage scratch;
	HRESULT hr = dx::LoadFromWICFile(wideName, dx::WIC_FLAGS_IGNORE_SRGB, nullptr, scratch);

	DXGI_FORMAT format = DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM;
	if (scratch.GetImage(0, 0, 0)->format != format)
	{
		dx::ScratchImage converted;
		hr = dx::Convert(
			*scratch.GetImage(0, 0, 0),
			format,
			dx::TEX_FILTER_DEFAULT,
			dx::TEX_THRESHOLD_DEFAULT,
			converted
		);

		mScratch = std::move(converted);
	}

	mScratch = std::move(scratch);
}

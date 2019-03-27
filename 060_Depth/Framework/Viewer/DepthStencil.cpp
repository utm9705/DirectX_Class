#include "Framework.h"
#include "DepthStencil.h"

DepthStencil::DepthStencil(UINT width, UINT height)
{
	D3DDesc desc;
	D3D::GetDesc(&desc);

	this->width = (width < 1) ? (UINT)desc.Width : width;
	this->height = (height < 1) ? (UINT)desc.Height : height;


	D3D11_TEXTURE2D_DESC textureDesc;
	{
		ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
		textureDesc.Width = this->width;
		textureDesc.Height = this->height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;

		HRESULT hr = D3D::GetDevice()->CreateTexture2D(&textureDesc, NULL, &texture);
		assert(SUCCEEDED(hr));
	}

	//Create DSV
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
		desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipSlice = 0;

		HRESULT hr = D3D::GetDevice()->CreateDepthStencilView(texture, &desc, &dsv);
		assert(SUCCEEDED(hr));
	}

	//Create SRV
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		desc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipLevels = 1;

		HRESULT hr = D3D::GetDevice()->CreateShaderResourceView(texture, &desc, &srv);
		assert(SUCCEEDED(hr));
	}
}

DepthStencil::~DepthStencil()
{
	SAFE_RELEASE(dsv);
	SAFE_RELEASE(texture);
	SAFE_RELEASE(srv);
}

void DepthStencil::SaveTexture(wstring saveFile)
{
	HRESULT hr = D3DX11SaveTextureToFile
	(
		D3D::GetDC(), texture, D3DX11_IFF_PNG, saveFile.c_str()
	);
	assert(SUCCEEDED(hr));
}
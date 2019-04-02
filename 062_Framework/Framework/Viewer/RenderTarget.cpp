#include "Framework.h"
#include "RenderTarget.h"

RenderTarget::RenderTarget(UINT width, UINT height)
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
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;

		HRESULT hr = D3D::GetDevice()->CreateTexture2D(&textureDesc, NULL, &texture);
		assert(SUCCEEDED(hr));
	}

	//Create RTV
	{
		D3D11_RENDER_TARGET_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
		desc.Format = textureDesc.Format;
		desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipSlice = 0;

		HRESULT hr = D3D::GetDevice()->CreateRenderTargetView(texture, &desc, &rtv);
		assert(SUCCEEDED(hr));
	}

	//Create SRV
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		desc.Format = textureDesc.Format;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipLevels = 1;

		HRESULT hr = D3D::GetDevice()->CreateShaderResourceView(texture, &desc, &srv);
		assert(SUCCEEDED(hr));
	}
}

RenderTarget::~RenderTarget()
{
	SAFE_RELEASE(rtv);
	SAFE_RELEASE(texture);
	SAFE_RELEASE(srv);
}

void RenderTarget::SaveTexture(wstring saveFile)
{
	HRESULT hr = D3DX11SaveTextureToFile
	(
		D3D::GetDC(), texture, D3DX11_IFF_PNG, saveFile.c_str()
	);
	assert(SUCCEEDED(hr));
}

void RenderTarget::Set(DepthStencil * dsv)
{
	D3D::Get()->SetRenderTarget(rtv, dsv->View());
	D3D::Get()->Clear(D3DXCOLOR(0, 0, 0, 1), rtv, dsv->View());
}

void RenderTarget::Sets(vector<RenderTarget *> & rtvs, DepthStencil * dsv)
{
	vector<ID3D11RenderTargetView *> views;
	for (UINT i = 0; i < rtvs.size(); i++)
		views.push_back(rtvs[i]->View());


	D3D::Get()->SetRenderTargets(views.size(), &views[0], dsv->View());

	for (UINT i = 0; i < views.size(); i++)
		D3D::Get()->Clear(D3DXCOLOR(0, 0, 0, 1), views[i], dsv->View());
}

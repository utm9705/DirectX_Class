#include "Framework.h"
#include "RenderTargetView.h"

RenderTargetView::RenderTargetView(UINT width, UINT height)
{
	//����Ÿ�� ũ�� ����
	{
		D3DDesc desc;
		D3D::GetDesc(&desc);

		this->width = (width < 1) ? (UINT)desc.Width : width;
		this->height = (height < 1) ? (UINT)desc.Height : height;
	}
	viewport = new Viewport((float)this->width, (float)this->height);


	D3D11_TEXTURE2D_DESC textureDesc;

	//Create Texture - RTV��
	{
		ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
		textureDesc.Width = this->width;
		textureDesc.Height = this->height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;

		HRESULT hr = D3D::GetDevice()->CreateTexture2D(&textureDesc, NULL, &rtvTexture);
		assert(SUCCEEDED(hr));
	}

	//Create RTV
	{
		D3D11_RENDER_TARGET_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
		desc.Format = textureDesc.Format;
		desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipSlice = 0;

		HRESULT hr = D3D::GetDevice()->CreateRenderTargetView(rtvTexture, &desc, &rtv);
		assert(SUCCEEDED(hr));
	}

	//Create SRV
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		desc.Format = textureDesc.Format;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipLevels = 1;

		HRESULT hr = D3D::GetDevice()->CreateShaderResourceView(rtvTexture, &desc, &srv);
		assert(SUCCEEDED(hr));
	}


	//Create Texture - DSV��
	{
		textureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

		HRESULT hr = D3D::GetDevice()->CreateTexture2D(&textureDesc, NULL, &dsvTexture);
		assert(SUCCEEDED(hr));
	}

	//Create DSV
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
		desc.Format = textureDesc.Format;
		desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipSlice = 0;

		HRESULT hr = D3D::GetDevice()->CreateDepthStencilView(dsvTexture, &desc, &dsv);
		assert(SUCCEEDED(hr));
	}
}

RenderTargetView::~RenderTargetView()
{
	SAFE_RELEASE(rtvTexture);
	SAFE_RELEASE(dsvTexture);

	SAFE_RELEASE(srv);
	SAFE_RELEASE(rtv);
	SAFE_RELEASE(dsv);

	SAFE_DELETE(viewport);
}

void RenderTargetView::Set(D3DXCOLOR clear)
{
	D3D::Get()->SetRenderTarget(rtv, dsv);
	D3D::Get()->Clear(clear, rtv, dsv);

	viewport->RSSetViewport();
}

void RenderTargetView::SaveRtvTexture(wstring saveFile)
{
	HRESULT hr = D3DX11SaveTextureToFile
	(
		D3D::GetDC(), rtvTexture, D3DX11_IFF_PNG, saveFile.c_str()
	);
	assert(SUCCEEDED(hr));
}

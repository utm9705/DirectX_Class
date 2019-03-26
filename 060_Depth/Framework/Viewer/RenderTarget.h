#pragma once

class RenderTarget
{
public:
	RenderTarget(UINT width = 0, UINT height = 0);
	~RenderTarget();

	void Set(D3DXCOLOR clear = 0xFF000000);

	ID3D11ShaderResourceView* GetRT_SRV() { return rtvSrv; }
	void SaveRtvTexture(wstring saveFile);

	ID3D11RenderTargetView* GetRTV() { return rtv; }
	ID3D11DepthStencilView* GetDSV() { return dsv; }

private:
	void CreateRTV();
	void CreateDSV();

private:
	UINT width, height;

	ID3D11Texture2D* rtvTexture;
	ID3D11RenderTargetView* rtv;
	ID3D11ShaderResourceView* rtvSrv;

	ID3D11Texture2D* dsvTexture;
	ID3D11DepthStencilView* dsv;
	ID3D11ShaderResourceView* dsvSrv;

	class Viewport* viewport;
};
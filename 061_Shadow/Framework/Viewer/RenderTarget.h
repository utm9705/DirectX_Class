#pragma once

class RenderTarget
{
public:
	RenderTarget(UINT width = 0, UINT height = 0);
	~RenderTarget();

	ID3D11ShaderResourceView* SRV() { return srv; }
	void SaveTexture(wstring saveFile);

	ID3D11RenderTargetView* View() { return rtv; }

	void Set(class DepthStencil* dsv);
	static void Sets(vector<RenderTarget *>& rtvs, class DepthStencil* dsv);

private:
	UINT width, height;

	ID3D11Texture2D* texture;
	ID3D11RenderTargetView* rtv;
	ID3D11ShaderResourceView* srv;
};
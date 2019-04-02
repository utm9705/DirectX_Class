#pragma once

class DepthStencil
{
public:
	DepthStencil(UINT width = 0, UINT height = 0);
	~DepthStencil();

	ID3D11ShaderResourceView* SRV() { return srv; }
	void SaveTexture(wstring saveFile);

	ID3D11DepthStencilView* View() { return dsv; }

private:
	UINT width, height;

	ID3D11Texture2D* texture;
	ID3D11DepthStencilView* dsv;
	ID3D11ShaderResourceView* srv;
};
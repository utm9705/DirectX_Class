#pragma once

class Render2D : public Renderer
{
public:
	Render2D(wstring shaderFile = L"");
	virtual ~Render2D();

	void Ready();

	virtual void Render();

	void SRV(ID3D11ShaderResourceView* srv);

private:
	D3DXMATRIX view;
	D3DXMATRIX orthographic;

	D3DXVECTOR2 position;
	D3DXVECTOR2 scale;

	ID3DX11EffectShaderResourceVariable* sMap;
};
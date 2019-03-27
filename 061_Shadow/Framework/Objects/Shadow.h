#pragma once

class Shadow
{
public:
	Shadow(Shader* shader, UINT width = 512, UINT height = 512);
	~Shadow();

	void Ready();
	void Update();
	void PreRender();
	void Render();
	void PostRender();

private:
	struct ShadowDepthDesc
	{
		D3DXMATRIX View;
		D3DXMATRIX Projection;
	} shadowDepthDesc;

	struct ShadowDesc
	{
		D3DXVECTOR2 MapSize;
		float Bias = 0.005f;

		float Padding;
	} shadowDesc;

private:
	Shader* shader;

	CBuffer* shadowDepthBuffer;
	ID3DX11EffectConstantBuffer* sShadowDepthBuffer;

	CBuffer* shadowBuffer;
	ID3DX11EffectConstantBuffer* sShadowBuffer;

	ID3DX11EffectShaderResourceVariable* sShadowMap;


	RenderTarget* depthTarget;
	DepthStencil* depthStencil;
	Viewport* viewport;


	class Fixity* lightCamera;
	class Perspective* lightPerspective;

	class Render2D* depthRender;
};
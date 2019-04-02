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
	void ShadowTransform();

private:
	struct ShadowDepthDesc
	{
		D3DXMATRIX View;
		D3DXMATRIX Projection;
	} shadowDepthDesc;

	struct ShadowDesc
	{
		D3DXVECTOR2 MapSize;
		float Bias = 0.0005f;

		float Padding;
	} shadowDesc;

private:
	Shader* shader;

	ConstantBuffer* shadowDepthBuffer;
	ID3DX11EffectConstantBuffer* sShadowDepthBuffer;

	ConstantBuffer* shadowBuffer;
	ID3DX11EffectConstantBuffer* sShadowBuffer;

	ID3DX11EffectShaderResourceVariable* sShadowMap;

	RenderTarget* depthTarget;
	DepthStencil* depthStencil;
	Viewport* viewport;

	D3DXVECTOR3 center;
	float radius;

	class Render2D* depthRender;

	ID3D11SamplerState* samplerState;
};
#pragma once

class Sky : public Renderer
{
public:
	Sky();
	~Sky();

	void Ready();
	void Update() override;

	void PreRender();
	void Render() override;
	void PostRender();

private:
	struct TargetDesc
	{
		D3DXVECTOR3 WaveLength = D3DXVECTOR3(0.65f, 0.57f, 0.475f);
		int SampleCount = 10;

		D3DXVECTOR3 InvWaveLength;
		float Padding;

		D3DXVECTOR3 WaveLengthMie;
		float Padding2;
	} targetDesc;

private:
	CBuffer* targetBuffer;
	ID3DX11EffectConstantBuffer* sTargetBuffer;
	class Render2D* render2D;

	RenderTargetView* mieTarget, *rayleighTarget;
	Texture* texture;
};
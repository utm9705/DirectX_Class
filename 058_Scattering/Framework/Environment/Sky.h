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

	struct NightDesc
	{
		float StarIntensity = 0.0f;
		float MoonAlpha = 0.0f;

		float Padding[2];
	} nightDesc;

	struct CloudDesc
	{
		float Tiles = 1.5f;
		float Cover = 0.005f;
		float Sharpness = 0.405f;
		float Speed = 0.05f;
	} cloudDesc;

private:
	bool realTime;
	float timeFactor;

	float theta, prevTheta, phi, prevPhi;


	class Scattering* scattering;
	class SkyDome* skyDome;
	class Cloud* cloud;
	class Moon* moon;


	CBuffer* targetBuffer;
	ID3DX11EffectConstantBuffer* sTargetBuffer;
	
	CBuffer* nightBuffer;
	ID3DX11EffectConstantBuffer* sNightBuffer;

	CBuffer* cloudBuffer;
	ID3DX11EffectConstantBuffer* sCloudBuffer;


	Texture* starMap;
	ID3DX11EffectShaderResourceVariable* sStarMap;
	ID3DX11EffectShaderResourceVariable* sRayleighMap;
	ID3DX11EffectShaderResourceVariable* sMieMap;

	ID3DX11EffectShaderResourceVariable* sCloudMap;

	Texture* moonMap;
	Texture* moonGlowMap;
	ID3DX11EffectShaderResourceVariable* sMoonMap;
};
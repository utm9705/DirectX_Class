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
		int SampleCount = 10; //높을수록 정밀해짐 대신 속도 다운

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
		float Tiles = 16.25f;
		float Cover = -0.1f;
		float Sharpness = 0.25f;

		float Padding;
	} cloudDesc;

private:
	bool realTime;
	float theta, phi;

	class Scattering* scattering;
	class SkyDome* skyDome;
	class Cloud* cloud;

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
};
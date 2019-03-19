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
	void CreateQuadData();
	void CreateSphereData();
	void CreateCloudData();

	void RenderCloud();

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
	bool realTime;

	float theta, phi;
	float prevTheta, prevPhi;


	CBuffer* targetBuffer;
	ID3DX11EffectConstantBuffer* sTargetBuffer;

	class Render2D* render2D;
	RenderTargetView* mieTarget, *rayleighTarget;


	VertexTexture* vertices;

	ID3D11Buffer* skyVB, *skyIB;
	UINT skyVC, skyIC;
	UINT radius, slices, stacks;

	ID3DX11EffectScalarVariable* sStarIntensity;

	Texture* starMap;
	ID3DX11EffectShaderResourceVariable* sStarMap;
	ID3DX11EffectShaderResourceVariable* sRayleighMap;
	ID3DX11EffectShaderResourceVariable* sMieMap;

	ID3D11Texture2D* cloudTexture;
	ID3D11ShaderResourceView* cloudSrv;
};
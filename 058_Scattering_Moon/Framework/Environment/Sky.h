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
	D3DXVECTOR3 GetDirection();


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
		float MoonAlpha = 1.0f;
		float Padding[2];
	} nightDesc;

	struct CloudDesc
	{
		float Tiles = 16.25f;
		float Cover = -0.1f;
		float Sharpress = 0.25f;

		float Padding;
	} cloudDesc;


private:
	float timeFactor;
	bool realTime;
	float theta, phi;
	float prevTheta, prevPhi;

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




	Texture* moonField;
	Texture* moonGlowField;

	ID3DX11EffectShaderResourceVariable* sMoonMap;

};


/*


void Moon::DrawMoonGlow()
{

D3DXVECTOR3 camPos;
D3DXMATRIX sc, rX, rY, dist, cam, matF;
values->MainCamera->Position(&camPos);
D3DXMatrixScaling(&sc, 50, 50, 1);
D3DXMatrixRotationX(&rX, -theta - (Math::PI * 0.5f));
D3DXMatrixRotationY(&rY, phi - (Math::PI * 0.5f));
D3DXMatrixTranslation(&cam, camPos.x, camPos.y, camPos.z);

if (theta < Math::PI * 0.5f || theta > Math::PI * 1.5f)
scatterBuffer->Data.MoonAlpha = (float)abs(sinf(theta + (float)Math::PI / 2.0f));
else
scatterBuffer->Data.MoonAlpha = 0.0f;

UINT stride = sizeof(VertexTexture);
UINT offset = 0;

{
// Moon
D3D::GetDC()->IASetVertexBuffers(0, 1, &quadBuffer, &stride, &offset);
D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

D3DXMatrixTranslation(&dist,
values->GlobalLight->Data.Direction.x * 815.0f,
values->GlobalLight->Data.Direction.y * 815.0f,
values->GlobalLight->Data.Direction.z * 815.0f);
matF = sc*rX*rY*dist*cam;

moonBuffer->SetMatrix(matF);
moonBuffer->SetVSBuffer(1);
scatterBuffer->SetPSBuffer(10);

moonField->SetShaderResource(10);
moonSampler->PSSetSamplers(10);

moonShader->Render();
D3D::GetDC()->Draw(6, 0);
}

{
// Moon Glow
D3D::GetDC()->IASetVertexBuffers(0, 1, &quadBuffer, &stride, &offset);
D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

D3DXMatrixScaling(&sc, 150, 150, 1);
D3DXMatrixTranslation(&dist,
values->GlobalLight->Data.Direction.x * 805.0f,
values->GlobalLight->Data.Direction.y * 805.0f,
values->GlobalLight->Data.Direction.z * 805.0f);
matF = sc*rX*rY*dist*cam;

moonBuffer->SetMatrix(matF);
moonBuffer->SetVSBuffer(1);
scatterBuffer->SetPSBuffer(10);

moonGlowField->SetShaderResource(10);
moonSampler->PSSetSamplers(10);

moonShader->Render();
D3D::GetDC()->Draw(6, 0);
}
}

*/
#include "Framework.h"
#include "Sky.h"
#include "Renders/Render2D.h"
#include "Viewer/RenderTargetView.h"

Sky::Sky()
	: Renderer()
{
	shader = new Shader(L"058_Scattering.fx");
	SetShader(shader);

	mieTarget = new RenderTargetView(128, 64);
	rayleighTarget = new RenderTargetView(128, 64);


	render2D = new Render2D();
	render2D->Scale(200, 200, 1);
	render2D->RotationDegree(0, 0, -90);


	VertexTexture vertices[6];
	vertices[0].Position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	vertices[1].Position = D3DXVECTOR3(-1.0f, +1.0f, 0.0f);
	vertices[2].Position = D3DXVECTOR3(+1.0f, -1.0f, 0.0f);
	vertices[3].Position = D3DXVECTOR3(+1.0f, -1.0f, 0.0f);
	vertices[4].Position = D3DXVECTOR3(-1.0f, +1.0f, 0.0f);
	vertices[5].Position = D3DXVECTOR3(+1.0f, +1.0f, 0.0f);

	vertices[0].Uv = D3DXVECTOR2(0, 1);
	vertices[1].Uv = D3DXVECTOR2(0, 0);
	vertices[2].Uv = D3DXVECTOR2(1, 1);
	vertices[3].Uv = D3DXVECTOR2(1, 1);
	vertices[4].Uv = D3DXVECTOR2(0, 0);
	vertices[5].Uv = D3DXVECTOR2(1, 0);

	CreateVertexBuffer(vertices, 6, sizeof(VertexTexture), 0);


	targetDesc.InvWaveLength.x = 1.0f / powf(targetDesc.WaveLength.x, 4.0f);
	targetDesc.InvWaveLength.y = 1.0f / powf(targetDesc.WaveLength.y, 4.0f);
	targetDesc.InvWaveLength.z = 1.0f / powf(targetDesc.WaveLength.z, 4.0f);

	targetDesc.WaveLengthMie.x = powf(targetDesc.WaveLength.x, -0.84f);
	targetDesc.WaveLengthMie.y = powf(targetDesc.WaveLength.y, -0.84f);
	targetDesc.WaveLengthMie.z = powf(targetDesc.WaveLength.z, -0.84f);

	targetBuffer = new CBuffer(&targetDesc, sizeof(TargetDesc));
	sTargetBuffer = shader->AsConstantBuffer("CB_Target");
}

Sky::~Sky()
{
}

void Sky::Ready()
{
	render2D->Ready();
}

void Sky::Update()
{
	Renderer::Update();
	
	render2D->Update();
	targetBuffer->Apply();
	sTargetBuffer->SetConstantBuffer(targetBuffer->Buffer());
}

void Sky::PreRender()
{
	mieTarget->Set();
	rayleighTarget->Set();

	ID3D11RenderTargetView* rtvs[2];
	rtvs[0] = rayleighTarget->GetRTV();
	rtvs[1] = mieTarget->GetRTV();

	ID3D11DepthStencilView* dsv;
	dsv = rayleighTarget->GetDSV();

	D3D::Get()->SetRenderTargets(2, rtvs, dsv);

	render2D->Render();

	Renderer::Render();
	shader->Draw(0, 0, 6);
}

void Sky::Render()
{
	
}

void Sky::PostRender()
{
	render2D->SRV(rayleighTarget->GetSRV());
	render2D->Position(0 + 100, 0 + 100, 0);
	render2D->Update();
	render2D->Render();

	render2D->SRV(mieTarget->GetSRV());
	render2D->Position(200 + 100, 0 + 100, 0);
	render2D->Update();
	render2D->Render();
}

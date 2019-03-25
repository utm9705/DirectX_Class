#include "Framework.h"
#include "Scattering.h"
#include "Renders/Render2D.h"
#include "Viewer/RenderTargetView.h"

Scattering::Scattering(Shader * shader)
	: shader(shader)
{
	mieTarget = new RenderTargetView(128, 64);
	rayleighTarget = new RenderTargetView(128, 64);

	render2D = new Render2D();
	render2D->Scale(200, 200, 1);
	render2D->RotationDegree(0, 0, -90);
}

Scattering::~Scattering()
{
	SAFE_DELETE(render2D);

	SAFE_DELETE(mieTarget);
	SAFE_DELETE(rayleighTarget);
}

void Scattering::Ready()
{
	render2D->Ready();

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

	Renderer::CreateVertexBuffer(vertices, 6, sizeof(VertexTexture), 0, &vertexBuffer);
}

void Scattering::Update()
{
	render2D->Update();
}

void Scattering::PostRender()
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

void Scattering::PreRender()
{
	mieTarget->Set();
	rayleighTarget->Set();

	ID3D11RenderTargetView* rtvs[2];
	rtvs[0] = rayleighTarget->GetRTV();
	rtvs[1] = mieTarget->GetRTV();

	ID3D11DepthStencilView* dsv;
	dsv = rayleighTarget->GetDSV();

	D3D::Get()->SetRenderTargets(2, rtvs, dsv);

	UINT stride = sizeof(VertexTexture);
	UINT offset = 0;

	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	shader->Draw(0, 0, 6);
}
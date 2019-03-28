#include "Framework.h"
#include "Shadow.h"
#include "Viewer/Fixity.h"
#include "Viewer/Perspective.h"
#include "Viewer/RenderTarget.h"
#include "Renders/Render2D.h"

Shadow::Shadow(Shader * shader, UINT width, UINT height)
	: shader(shader)
	, center(0, 0, 0), radius(100)
{
	shadowDepthBuffer = new CBuffer(&shadowDepthDesc, sizeof(ShadowDepthDesc));
	sShadowDepthBuffer = shader->AsConstantBuffer("CB_ShadowDepth");

	shadowBuffer = new CBuffer(&shadowDesc, sizeof(ShadowDesc));
	sShadowBuffer = shader->AsConstantBuffer("CB_Shadow");

	sShadowMap = shader->AsShaderResource("ShadowMap");


	depthTarget = new RenderTarget(width, height);
	depthStencil = new DepthStencil(width, height);
	viewport = new Viewport(width, height);

	shadowDesc.MapSize = D3DXVECTOR2((float)width, (float)height);

	depthRender = new Render2D();
	depthRender->Scale(200, 200, 1);
	depthRender->Position(100, 100, 0);


	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.MinLOD = 0;
	samplerDesc.MinLOD = FLT_MAX;

	HRESULT hr = D3D::GetDevice()->CreateSamplerState(&samplerDesc, &samplerState);
	assert(SUCCEEDED(hr));

	//shader->AsSampler("ShadowSampler")->SetSampler(0, samplerState);
}

Shadow::~Shadow()
{
	SAFE_RELEASE(samplerState);

	SAFE_DELETE(shadowDepthBuffer);
	SAFE_DELETE(shadowBuffer);
}

void Shadow::Ready()
{
	depthRender->Ready();
}

void Shadow::Update()
{
	depthRender->Update();

	ImGui::SliderFloat("Shadow Bias", &shadowDesc.Bias, 0.00001f, 0.1f, "%.5f");

	ImGui::SliderFloat3("Direction", (float *)&Context::Get()->LightDirection(), -1, 1);
	ImGui::SliderFloat3("Center", (float *)&center, -20, 20);
	ImGui::SliderFloat("Radius", &radius, 1, 200);


}

void Shadow::PreRender()
{
	ShadowTransform();

	depthTarget->Set(depthStencil);
	viewport->RSSetViewport();

	shadowDepthBuffer->Apply();
	sShadowDepthBuffer->SetConstantBuffer(shadowDepthBuffer->Buffer());
}

void Shadow::Render()
{
	shadowBuffer->Apply();
	sShadowBuffer->SetConstantBuffer(shadowBuffer->Buffer());

	sShadowMap->SetResource(depthStencil->SRV());
}


void Shadow::PostRender()
{
	depthRender->SRV(depthTarget->SRV());
	depthRender->Render();
}

void Shadow::ShadowTransform()
{
	D3DXVECTOR3 up = D3DXVECTOR3(0, 1, 0);
	D3DXVECTOR3 direction = Context::Get()->LightDirection();
	D3DXVECTOR3 position = -2.0f * radius * direction;
	D3DXVECTOR3 target = center;

	D3DXMatrixLookAtLH(&shadowDepthDesc.View, &position, &target, &up);

	D3DXVECTOR3 cube;
	D3DXVec3TransformCoord(&cube, &target, &shadowDepthDesc.View);

	float l = cube.x - radius;
	float b = cube.y - radius;
	float n = cube.z - radius;

	float r = cube.x + radius;
	float t = cube.y + radius;
	float f = cube.z + radius;

	D3DXMatrixOrthoLH(&shadowDepthDesc.Projection, r - l, t - b, n, f);
}

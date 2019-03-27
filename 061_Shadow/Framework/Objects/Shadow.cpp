#include "Framework.h"
#include "Shadow.h"
#include "Viewer/Fixity.h"
#include "Viewer/Perspective.h"
#include "Viewer/RenderTarget.h"
#include "Renders/Render2D.h"

Shadow::Shadow(Shader * shader, UINT width, UINT height)
	: shader(shader)
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


	lightCamera = new Fixity();
	lightCamera->Position(0, 40, -50);
	lightPerspective = new Perspective(1, 1, Math::PI * 0.5f, 1, 1000);
}

Shadow::~Shadow()
{
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

	D3DXVECTOR3 position;
	lightCamera->Position(&position);

	ImGui::SliderFloat3("Light Position", (float *)&position, -100, 100);
	lightCamera->Position(position);

	D3DXVECTOR2 degree;
	lightCamera->RotationDegree(&degree);

	ImGui::SliderFloat2("Light Rotation", (float *)&degree, -179, 179);
	lightCamera->RotationDegree(degree);

	ImGui::SliderFloat("Shadow Bias", &shadowDesc.Bias, 0.00001f, 0.1f, "%.5f");
}

void Shadow::PreRender()
{
	depthTarget->Set(depthStencil);
	viewport->RSSetViewport();

	shadowDepthDesc.View = lightCamera->Matrix();
	shadowDepthDesc.Projection = lightPerspective->Matrix();

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

#include "Framework.h"
#include "Shadow.h"
#include "Viewer/Fixity.h"
#include "Viewer/Perspective.h"
#include "Viewer/RenderTarget.h"
#include "Renders/Render2D.h"

Shadow::Shadow(Shader * shader, UINT width, UINT height)
	: shader(shader)
{
	matrixBuffer = new CBuffer(&matrixDesc, sizeof(MatrixDesc));
	sMatrixBuffer = shader->AsConstantBuffer("CB_ShadowDepth");

	depthTarget = new RenderTarget(width, height);
	depthStencil = new DepthStencil(width, height);
	viewport = new Viewport(width, height);

	
	depthRender = new Render2D();
	depthRender->Scale(200, 200, 1);
	depthRender->Position(100, 100, 0);

	lightCamera = new Fixity();
	lightCamera->Position(0, 10, -20);
	lightPerspective = new Perspective(1, 1, Math::PI * 0.5f, 1, 1000);
}

Shadow::~Shadow()
{
	SAFE_DELETE(matrixBuffer);
}

void Shadow::Ready()
{
	depthRender->Ready();
}

void Shadow::PreRender()
{
	depthTarget->Set(depthStencil);
	viewport->RSSetViewport();


	matrixDesc.View = lightCamera->Matrix();
	matrixDesc.Projection = lightPerspective->Matrix();

	matrixBuffer->Apply();
	sMatrixBuffer->SetConstantBuffer(matrixBuffer->Buffer());
}

void Shadow::Update()
{
	depthRender->Update();

	D3DXVECTOR3 position;
	lightCamera->Position(&position);
	ImGui::SliderFloat3("Light Position", (float*)&position, -100, 100);
	lightCamera->Position(position);


	D3DXVECTOR2 degree;
	lightCamera->RotationDegree(&degree);
	ImGui::SliderFloat2("Light Rotation", (float*)&degree, -179, 179);
	lightCamera->RotationDegree(degree);
}

void Shadow::PostRender()
{
	depthRender->SRV(depthTarget->SRV());
	depthRender->Render();
}

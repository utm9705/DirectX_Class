#include "Framework.h"
#include "Shadow.h"
#include "Viewer/Fixity.h"
#include "Viewer/Perspective.h"
#include "Viewer/RenderTarget.h"
#include "Renders/Render2D.h"

Shadow::Shadow(Shader * shader)
	: shader(shader)
{
	matrixBuffer = new CBuffer(&matrixDesc, sizeof(MatrixDesc));
	sMatrixBuffer = shader->AsConstantBuffer("CB_ShadowDepth");

	depthTarget = new RenderTarget(2048, 2048);
	
	depthRender = new Render2D();
	depthRender->Scale(200, 200, 1);
	depthRender->Position(100, 100, 0);


	lightCamera = new Fixity();
	lightCamera->Position(1, 1, 1);
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
	depthTarget->Set();

	matrixDesc.View = lightCamera->Matrix();
	matrixDesc.Projection = lightPerspective->Matrix();

	matrixBuffer->Apply();
	sMatrixBuffer->SetConstantBuffer(matrixBuffer->Buffer());
}

void Shadow::Update()
{
	depthRender->Update();
}

void Shadow::PostRender()
{
	depthRender->SRV(depthTarget->GetRT_SRV());
	depthRender->Render();
}

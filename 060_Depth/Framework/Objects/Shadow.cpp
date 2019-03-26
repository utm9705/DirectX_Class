#include "Framework.h"
#include "Shadow.h"
#include "Viewer/Fixity.h"
#include "Viewer/Perspective.h"
#include "Viewer/RenderTarget.h"
#include "Renders/Render2D.h"

Shadow::Shadow(Shader * shader)
	: shader(shader)
{
	matrixBuffer = new CBuffer(&matrixBuffer, sizeof(MatrixDesc));
	sMatrixBuffer = shader->AsConstantBuffer("CB_ShadowDepth");

	depthTarget = new RenderTarget(2048, 2048);

	depthRender = new Render2D();
	depthRender->Scale(200, 200, 1);
	depthRender->Position(100, 100, 0);

	lightCamera = new Fixity();
	lightCamera->Position(1, 1, 1);
	lightPerspective = new Perspective(1, 1, Math::PI * 0.5f, 1, 1000); //ortho와 다른점은 이건 깊이가 들어간다
}

Shadow::~Shadow()
{
	SAFE_DELETE(matrixBuffer);
	SAFE_DELETE(depthTarget);
	SAFE_DELETE(lightCamera);
	SAFE_DELETE(lightPerspective);
	SAFE_DELETE(depthRender);
}

void Shadow::Ready()
{
	depthRender->Ready();
}

void Shadow::Update()
{
	depthRender->Update();
}

void Shadow::PreRender()
{
	depthTarget->Set();

	matrixDesc.View = lightCamera->Matrix();
	matrixDesc.Projection = lightPerspective->Matrix();

	matrixBuffer->Apply();
	sMatrixBuffer->SetConstantBuffer(matrixBuffer->Buffer());
}

void Shadow::PostRender()
{
	depthRender->SRV(depthTarget->GetRT_SRV());
	depthRender->Render();
}

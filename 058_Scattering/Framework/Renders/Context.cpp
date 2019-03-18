#include "Framework.h"
#include "Context.h"
#include "Viewer/Viewport.h"
#include "Viewer/Perspective.h"
#include "Viewer/Freedom.h"

Context* Context::instance = NULL;

Context * Context::Get()
{
	//assert(instance != NULL);

	return instance;
}

void Context::Create()
{
	assert(instance == NULL);

	instance = new Context();
}

void Context::Delete()
{
	SAFE_DELETE(instance);
}

Context::Context()
{
	D3DDesc desc;
	D3D::GetDesc(&desc);

	camera = new Freedom();
	perspective = new Perspective(desc.Width, desc.Height);
	viewport = new Viewport(desc.Width, desc.Height);

	lightColor = D3DXCOLOR(0, 0, 0, 1);
	lightDirection = D3DXVECTOR3(-1, -1, 1);
	lightPosition = D3DXVECTOR3(0, 0, 0);
}

Context::~Context()
{
	SAFE_DELETE(camera);
	SAFE_DELETE(perspective);
	SAFE_DELETE(viewport);
}

void Context::Update()
{
	ImGui::SliderFloat3("Light Direction", (float*)&lightDirection, -1, 1);

	camera->Update();
}

void Context::Render()
{
	viewport->RSSetViewport();
}

D3DXMATRIX& Context::View()
{
	return camera->Matrix();
}

D3DXMATRIX& Context::Projection()
{
	return perspective->Matrix();
}
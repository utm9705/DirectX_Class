#include "framework.h"
#include "CubeSky.h"
#include "../Viewer/Camera.h"

CubeSky::CubeSky(wstring cubeFile)
{
	shader = new Shader(L"026_CubeSky.fx");
	sphere = new MeshSphere(shader, 500, 30, 30);

	cubeFile = L"../../_Textures/" + cubeFile;
	HRESULT hr = D3DX11CreateShaderResourceViewFromFile
	(
		D3D::GetDevice(), cubeFile.c_str(), NULL, NULL, &srv, NULL
	);
	assert(SUCCEEDED(hr));

	shader->AsSRV("CubeMap")->SetResource(srv);
}

CubeSky::~CubeSky()
{
	SAFE_RELEASE(srv);
	SAFE_DELETE(sphere);
}

void CubeSky::Update()
{
	D3DXVECTOR3 position;
	Context::Get()->GetCamera()->Position(&position);

	sphere->GetTransform()->Position(position);
	sphere->Update();
}

void CubeSky::Render()
{
	sphere->Render();
}

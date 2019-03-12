#include "framework.h"
#include "CubeSky.h"
#include "../Viewer/Camera.h"

CubeSky::CubeSky(Shader* shader, wstring cubeFile)
{
	sphere = new MeshSphere(shader, 500, 30, 30);

	cubeFile = L"../../_Textures/" + cubeFile;
	HRESULT hr = D3DX11CreateShaderResourceViewFromFile
	(
		D3D::GetDevice(), cubeFile.c_str(), NULL, NULL, &srv, NULL
	);
	assert(SUCCEEDED(hr));

	shader->AsShaderResource("CubeMap")->SetResource(srv);
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

	sphere->Position(position);
	sphere->Update();
}

void CubeSky::Render()
{
	sphere->Render();
}

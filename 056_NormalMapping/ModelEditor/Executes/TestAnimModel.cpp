#include "stdafx.h"
#include "TestAnimModel.h"
#include "Assimp/Loader.h"

void TestAnimModel::Initialize()
{
	Loader* loader = NULL;

	/*loader = new Loader
	(
		Assets + L"Kachujin/Mesh.fbx",
		Models + L"Kachujin/", L"Kachujin"
	);
	loader->ExportMaterial();
	loader->ExportMesh();
	SAFE_DELETE(loader);*/


	/*loader = new Loader
	(
		Assets + L"Kachujin/Samba_Dancing.fbx",
		Models + L"Kachujin/", L"Samba_Dancing"
	);*/

	loader = new Loader
	(
		Assets + L"Kachujin/Idle.fbx",
		Models + L"Kachujin/", L"Idle"
	);
	loader->ExportAnimation(0);
	SAFE_DELETE(loader);

	count = 0;
}

void TestAnimModel::Ready()
{
	model = new Model();
	model->ReadMaterial(Models + L"Kachujin/Kachujin.material");
	model->ReadMesh(Models + L"Kachujin/Kachujin.mesh");
	clip = new ModelClip(Models + L"Kachujin/Idle.animation");

	modelAnimInstance = new ModelAnimInstance(model, clip, L"054_ModelAnimInstance.fx");
	modelAnimInstance->Ready();

	/*D3DXMATRIX S, T;
	float scale;

	for (int i = 0; i < 100; i++)
	{
		scale = Math::Random(0.02f, 0.07f);
		D3DXMatrixScaling(&S, scale, scale, scale);
		D3DXMatrixTranslation(&T, Math::Random(-100.0f, 100.0f), 0, Math::Random(-100.0f, 100.0f));

		D3DXMATRIX world = S * T;
		modelAnimInstance->AddWorld(world);
	}*/
}

void TestAnimModel::Destroy()
{
	SAFE_DELETE(modelAnimInstance);
	SAFE_DELETE(model);
}

void TestAnimModel::Update()
{
	ImGui::LabelText("FPS", "%d", (int)ImGui::GetIO().Framerate);

	if (Keyboard::Get()->Down(VK_SPACE) && count < 100)
	{
		D3DXMATRIX S, T;
		float scale;

		scale = Math::Random(0.02f, 0.07f);
		D3DXMatrixScaling(&S, scale, scale, scale);
		D3DXMatrixTranslation(&T, Math::Random(-100.0f, 100.0f), 0, Math::Random(-100.0f, 100.0f));

		D3DXMATRIX world = S * T;
		modelAnimInstance->AddWorld(world);

		count++;
	}
}

void TestAnimModel::PreRender()
{

}

void TestAnimModel::Render()
{
	modelAnimInstance->Render();
}
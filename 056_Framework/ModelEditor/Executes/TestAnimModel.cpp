#include "stdafx.h"
#include "TestAnimModel.h"
#include "Assimp/Loader.h"

void TestAnimModel::Initialize()
{
	Loader* loader = NULL;

	loader = new Loader();
	loader->ReadFile(L"Kachujin/Mesh.fbx");
	loader->ExportMaterial(L"Kachujin/Mesh");
	loader->ExportMesh(L"Kachujin/Mesh");
	SAFE_DELETE(loader);

	loader = new Loader();
	loader->ReadFile(L"Kachujin/Idle.fbx");
	loader->ExportAnimation(0, L"Kachujin/Idle");
	SAFE_DELETE(loader);

	loader = new Loader();
	loader->ReadFile(L"Kachujin/Samba_Dancing.fbx");
	loader->ExportAnimation(0, L"Kachujin/Samba_Dancing");
	SAFE_DELETE(loader);
}

void TestAnimModel::Ready()
{
	model = new Model();
	model->ReadMaterial( L"Kachujin/Mesh");
	model->ReadMesh(L"Kachujin/Mesh");
	clip = new ModelClip( L"Kachujin/Idle");

	shader = new Shader(L"054_ModelAnimInstance.fx");
	modelAnimInstance = new ModelAnimInstance(model, clip, shader);
	modelAnimInstance->Ready();

	D3DXMATRIX S, T;
	float scale;

	for (int i = 0; i < 10; i++)
	{
		scale = Math::Random(0.02f, 0.07f);
		D3DXMatrixScaling(&S, scale, scale, scale);
		D3DXMatrixTranslation(&T, Math::Random(-100.0f, 100.0f), 0, Math::Random(-100.0f, 100.0f));

		D3DXMATRIX world = S * T;
		modelAnimInstance->AddWorld(world);
	}
}

void TestAnimModel::Destroy()
{
	SAFE_DELETE(shader);
	SAFE_DELETE(modelAnimInstance);
	SAFE_DELETE(model);
}

void TestAnimModel::Update()
{
	//ImGui::LabelText("FPS", "%d", (int)ImGui::GetIO().Framerate);

	//if (Keyboard::Get()->Down(VK_SPACE) && count < 100)
	//{
	//	D3DXMATRIX S, T;
	//	float scale;

	//	scale = Math::Random(0.02f, 0.07f);
	//	D3DXMatrixScaling(&S, scale, scale, scale);
	//	D3DXMatrixTranslation(&T, Math::Random(-100.0f, 100.0f), 0, Math::Random(-100.0f, 100.0f));

	//	D3DXMATRIX world = S * T;
	//	modelAnimInstance->AddWorld(world);

	//	count++;
	//}

	modelAnimInstance->Update();
}

void TestAnimModel::PreRender()
{

}

void TestAnimModel::Render()
{
	modelAnimInstance->Render();
}
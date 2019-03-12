#include "stdafx.h"
#include "TestModel.h"
#include "Assimp/Loader.h"

void TestModel::Initialize()
{
	Loader* loader = NULL;

	//Tank/Tank.fbx
	loader = new Loader();
	loader->ReadFile(L"Tank/Tank.fbx");
	loader->ExportMaterial(L"Tank/Tank", false);
	loader->ExportMesh(L"Tank/Tank");
	SAFE_DELETE(loader);
}

void TestModel::Ready()
{
	model = new Model();
	model->ReadMaterial(L"Tank/Tank");
	model->ReadMesh(L"Tank/Tank");

	shader = new Shader(L"053_ModelInstance.fx");
	modelInstance = new ModelInstance(model, shader);
	modelInstance->Ready();

	D3DXMATRIX S, T;
	float scale;

	for (int i = 0; i < 100; i++)
	{
		scale = Math::Random(0.2f, 0.7f);
		D3DXMatrixScaling(&S, scale, scale, scale);
		D3DXMatrixTranslation(&T, Math::Random(-20.0f, 20.0f), 0, Math::Random(-20.0f, 20.0f));

		D3DXMATRIX world = S * T;
		modelInstance->AddWorld(world);
	}
}

void TestModel::Destroy()
{
	SAFE_DELETE(shader);
	SAFE_DELETE(modelInstance);
	SAFE_DELETE(model);
}

void TestModel::Update()
{
	ImGui::LabelText("FPS", "%d", (int)ImGui::GetIO().Framerate);

	modelInstance->Update();
}

void TestModel::PreRender()
{

}

void TestModel::Render()
{
	modelInstance->Render();
}
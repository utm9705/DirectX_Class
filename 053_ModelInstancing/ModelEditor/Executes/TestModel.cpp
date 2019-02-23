#include "stdafx.h"
#include "TestModel.h"
#include "Assimp/Loader.h"

void TestModel::Initialize()
{
	Loader* loader = NULL;

	//Tank/Tank.fbx
	loader = new Loader
	(
		Assets + L"Tank/Tank.fbx",
		Models + L"Tank/", L"Tank"
	);
	//loader->ExportMaterial();
	loader->ExportMesh();
	SAFE_DELETE(loader);
}

void TestModel::Ready()
{
	model = new Model();
	model->ReadMaterial(Models + L"Tank/Tank.material");
	model->ReadMesh(Models + L"Tank/Tank.mesh");


	modelInstance = new ModelInstance(model, L"054_ModelInstance.fx");
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
	SAFE_DELETE(modelInstance);
	SAFE_DELETE(model);
}

void TestModel::Update()
{
	ImGui::LabelText("FPS", "%d", (int)ImGui::GetIO().Framerate);
}

void TestModel::PreRender()
{

}

void TestModel::Render()
{
	modelInstance->Render();
}
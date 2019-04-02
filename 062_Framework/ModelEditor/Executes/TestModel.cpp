#include "stdafx.h"
#include "TestModel.h"
#include "Assimp/Loader.h"

void TestModel::Initialize()
{
	Loader* loader = NULL;

	////Tank/Tank.fbx
	//loader = new Loader();
	//loader->ReadFile(L"Tank/Tank.fbx");
	//loader->ExportMaterial(L"Tank/Tank", false);
	//loader->ExportMesh(L"Tank/Tank");
	//SAFE_DELETE(loader);

	//Paladin/Mesh.fbx
	loader = new Loader();
	loader->ReadFile(L"Paladin/Mesh.fbx");
	loader->ExportMaterial(L"Paladin/Mesh");
	loader->ExportMesh(L"Paladin/Mesh");
	SAFE_DELETE(loader);
}

void TestModel::Ready()
{
	model = new Model();
	model->ReadMaterial(L"Paladin/Mesh");
	model->ReadMesh(L"Paladin/Mesh");

	/*model = new Model();
	model->ReadMaterial(L"Tank/Tank");
	model->ReadMesh(L"Tank/Tank");*/

	/*shader = new Shader(L"057_Model.fx");
	modelInstance = new ModelInstance(model, shader);
	modelInstance->Ready();
	

	D3DXMATRIX S, T;
	float scale;

	for (int i = 0; i < 100; i++)
	{
		scale = Math::Random(0.01f, 0.05f);
		D3DXMatrixScaling(&S, scale, scale, scale);
		D3DXMatrixTranslation(&T, Math::Random(-20.0f, 20.0f), 0, Math::Random(-20.0f, 20.0f));

		D3DXMATRIX world = S * T;
		modelInstance->AddWorld(world);
	}*/
}

void TestModel::Destroy()
{
	SAFE_DELETE(shader);
	SAFE_DELETE(modelRender);
	SAFE_DELETE(model);
}

void TestModel::Update()
{
	//modelRender->Update();
}

void TestModel::PreRender()
{

}

void TestModel::Render()
{
	//modelRender->Render();
}
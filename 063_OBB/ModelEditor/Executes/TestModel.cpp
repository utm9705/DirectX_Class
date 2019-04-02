#include "stdafx.h"
#include "TestModel.h"
#include "Assimp/Loader.h"

void TestModel::Initialize()
{
	
}

void TestModel::Ready()
{
	//Tower
	ExportTower();
	tower = NULL;
	CreateTower();

	//Tank
	ExportTank();
	tank = NULL;
	CreateTank();

	////Hanjo
	ExportHanjo();
	hanjo = NULL;
	CreateHanjo();
}

void TestModel::Destroy()
{
	SAFE_DELETE(hanjoModel);
	SAFE_DELETE(hanjoShader);
	SAFE_DELETE(hanjo);


	SAFE_DELETE(tankModel);
	SAFE_DELETE(tankShader);
	SAFE_DELETE(tank);


	SAFE_DELETE(towerModel);
	SAFE_DELETE(towerShader);
	SAFE_DELETE(tower);
}

void TestModel::Update()
{
	if (tower != NULL) tower->Update();
	//if (tank != NULL) tank->Update();
	if (hanjo != NULL) hanjo->Update();


	//D3DXVECTOR3 position;
	//hanjo->GetTransform(0)->Position(&position);

	//Keyboard::Get()->MovePosition(&position, 10);
	//hanjo->GetTransform(0)->Position(position);

	//hanjo->UpdateTransforms();
}

void TestModel::PreRender()
{
}

void TestModel::Render()
{
	if (tower != NULL) tower->Render();
	//if (tank != NULL) tank->Render();
	if (hanjo != NULL) hanjo->Render();
}

void TestModel::ExportTower()
{
	Loader* loader = new Loader();
	loader->ReadFile(L"Tower/Tower.fbx");
	loader->ExportMaterial(L"Tower/Tower");
	loader->ExportMesh(L"Tower/Tower");
}

void TestModel::CreateTower()
{
	towerModel = new Model();
	towerModel->ReadMaterial(L"Tower/Tower");
	towerModel->ReadMesh(L"Tower/Tower");

	towerShader = new Shader(L"063_Model_Instance.fx");
	tower = new ModelRender(towerShader, towerModel);

	for (UINT i = 0; i < 100; i++)
	{
		UINT index = tower->AddTransform();
		Transform* transform = tower->GetTransform(index);

		D3DXVECTOR3 position = Math::RandomVec3(-150.0f, 150.0f);
		position.y = 0.0f;

		D3DXVECTOR3 scale = Math::RandomVec3(0.05f, 0.08f);
		D3DXVECTOR3 rotation = Math::RandomVec3(0.0f, 180.0f);

		transform->Position(position);
		transform->Scale(scale);
		transform->RotationDegree(0, Math::Random(0.0f, 359.0f), 0);
	}
	tower->UpdateTransforms();
}

void TestModel::ExportTank()
{
	Loader* loader = new Loader();
	loader->ReadFile(L"Tank/Tank.fbx");
	loader->ExportMaterial(L"Tank/Tank", false);
	loader->ExportMesh(L"Tank/Tank");
}

void TestModel::CreateTank()
{
	tankModel = new Model();
	tankModel->ReadMaterial(L"Tank/Tank");
	tankModel->ReadMesh(L"Tank/Tank");

	tankShader = new Shader(L"063_Model_Instance.fx");
	tank = new ModelRender(tankShader, tankModel);
	for (UINT i = 0; i < 100; i++)
	{
		UINT index = tank->AddTransform();
		Transform* transform = tank->GetTransform(index);

		D3DXVECTOR3 position((float)i * 5, 0, 0);

		D3DXVECTOR3 scale = Math::RandomVec3(2, 2);
		D3DXVECTOR3 rotation = Math::RandomVec3(0.0f, 180.0f);

		transform->Position(position);
		transform->Scale(scale);
		transform->RotationDegree(0, Math::Random(0.0f, 359.0f), 0);
	}
	tank->UpdateTransforms();
}

void TestModel::ExportHanjo()
{
	Loader* loader = new Loader();
	loader->ReadFile(L"Kachujin/Mesh.fbx");
	loader->ExportMaterial(L"Kachujin/Mesh");
	loader->ExportMesh(L"Kachujin/Mesh");
}

void TestModel::CreateHanjo()
{
	hanjoModel = new Model();
	hanjoModel->ReadMaterial(L"Kachujin/Mesh");
	hanjoModel->ReadMesh(L"Kachujin/Mesh");

	hanjoShader = new Shader(L"063_Model_Instance.fx");
	hanjo = new ModelRender(hanjoShader, hanjoModel);

	for (UINT i = 0; i < 100; i++)
	{
		UINT index = hanjo->AddTransform();
		Transform* transform = hanjo->GetTransform(index);

		D3DXVECTOR3 position = Math::RandomVec3(-100.0f, 100.0f);
		position.y = 0.0f;

		D3DXVECTOR3 scale = Math::RandomVec3(0.03f, 0.05f);

		transform->Position(position);
		transform->Scale(scale);
		transform->RotationDegree(0, Math::Random(0.0f, 359.0f), 0);
	}
	hanjo->UpdateTransforms();
}

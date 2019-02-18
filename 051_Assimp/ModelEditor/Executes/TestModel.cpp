#include "stdafx.h"
#include "TestModel.h"
#include "Assimp/Loader.h"

void TestModel::Initialize()
{
	Loader* loader = NULL;

	//Tank.fbx
	loader = new Loader
	(
		Assets + L"Tank/Tank.fbx", Models + L"Tank/", L"Tank"
	);
	loader->ExportMaterial();
	loader->ExportMesh();
	SAFE_DELETE(loader);


	////Kachujin.fbx
	//loader = new Loader
	//(
	//	Assets + L"Kachujin/Mesh.fbx", Models + L"Mesh/", L"Mesh"
	//);
	//loader->ExportMaterial();
	//loader->ExportMesh();
	//SAFE_DELETE(loader);


	////Car.fbx
	//loader = new Loader
	//(
	//	Assets + L"Car/Car.obj", Models + L"Car/", L"Car"
	//);
	//loader->ExportMaterial();
	////loader->ExportMesh();
	//SAFE_DELETE(loader);


	////Tower.fbx
	//loader = new Loader
	//(
	//	Assets + L"Tower/Tower.fbx", Models + L"Tower/", L"Tower"
	//);
	//loader->ExportMaterial();
	//loader->ExportMesh();
	//SAFE_DELETE(loader);
}

void TestModel::Ready()
{
	model = new GameModel
	(
		Shaders + L"046_Model.fx",
		Models + L"Tank/Tank.material",
		Models + L"Tank/Tank.mesh"
	);
	model->Ready();
	//model->Scale(0.1f, 0.1f, 0.1f);
}

void TestModel::Destroy()
{
}

void TestModel::Update()
{
	model->Update();
}

void TestModel::PreRender()
{

}

void TestModel::Render()
{
	model->Render();
}
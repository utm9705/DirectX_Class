#include "stdafx.h"
#include "TestAnimator.h"
#include "Fbx/FbxLoader.h"

void TestAnimator::Initialize()
{
	FbxLoader* loader = NULL;
	vector<wstring> clipList;

	/*loader = new FbxLoader
	(
	Assets + L"Paladin/Mesh.fbx",
	Models + L"Paladin/", L"Paladin"
	);
	loader->ExportMaterial();
	loader->ExportMesh();
	loader->ExportAnimation(0);

	loader->GetClipList(&clipList);
	SAFE_DELETE(loader);


	gameModel = new GameModel
	(
	Shaders + L"039_Model.fx",
	Models + L"Paladin/Paladin.material",
	Models + L"Paladin/Paladin.mesh"
	);*/

	/*gameModel = new GameModel
	(
	Shaders + L"039_Model.fx",
	Models + L"Paladin/Paladin.material",
	Models + L"Paladin/Paladin.mesh"
	);*/

	/*
	loader = new FbxLoader
	(
		Assets + L"Kachujin/Mesh.fbx",
		Models + L"Kachujin/", L"Kachujin"
	);
	loader->ExportMaterial();
	loader->ExportMesh();
	SAFE_DELETE(loader);

	loader = new FbxLoader
	(
		Assets + L"Kachujin/Idle.fbx",
		Models + L"Kachujin/", L"Idle"
	);
	loader->ExportAnimation(0);
	SAFE_DELETE(loader);*/
}

void TestAnimator::Ready()
{
	gameModel = new GameAnimator
	(
		Shaders + L"046_Model.fx",
		Models + L"Kachujin/Kachujin.material",
		Models + L"Kachujin/Kachujin.mesh"
	);
	gameModel->AddClip(Models + L"Kachujin/Idle.animation");
	gameModel->Ready();

	gameModel->Scale(0.01f, 0.01f, 0.01f);
}

void TestAnimator::Destroy()
{
	
}

void TestAnimator::Update()
{
	gameModel->Update();
}

void TestAnimator::PreRender()
{

}

void TestAnimator::Render()
{
	gameModel->Render();
}
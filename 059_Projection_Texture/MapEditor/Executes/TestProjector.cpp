#include "stdafx.h"
#include "TestProjector.h"
#include "Viewer/Projector.h"

void TestProjector::Initialize()
{
	shader = new Shader(L"059_Projector.fx");
}

void TestProjector::Ready()
{
	grid = new MeshGrid(shader);
	grid->Scale(10, 1, 10);
	grid->Position(0, 0, 0);
	grid->DiffuseMap(L"Stones.png");

	cube = new MeshCube(shader);
	cube->Scale(1, 1, 1);
	cube->Position(0, 2.5f, 0);
	cube->DiffuseMap(L"Box.Png");

	projector = new Projector(shader, L"Terrain/MagicCircle.png");
}

void TestProjector::Destroy()
{
	
}

void TestProjector::Update()
{
	projector->Update();
	grid->Update();
	cube->Update();
}

void TestProjector::PreRender()
{
	
}

void TestProjector::Render()
{
	grid->Render();
	cube->Render();
}

void TestProjector::PostRender()
{
	
}

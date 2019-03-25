#include "stdafx.h"
#include "TestDepth.h"
#include "Environment/Sky.h"
#include "Environment/Terrain.h"

void TestDepth::Initialize()
{
	gridShader = new Shader(L"059_Depth.fx");
}

void TestDepth::Ready()
{
	grid = new MeshGrid(gridShader);
	grid->Scale(10, 1, 10);
	grid->Position(0, 0, 0);
}

void TestDepth::Destroy()
{
	
}

void TestDepth::Update()
{
	grid->Update();
}

void TestDepth::PreRender()
{
	
}

void TestDepth::Render()
{
	grid->Render();
}

void TestDepth::PostRender()
{
	
}

#include "stdafx.h"
#include "TestRect.h"
#include "Environment/CubeSky.h"
#include "Environment/Terrain.h"

void TestRect::Initialize()
{
	skyShader = new Shader(L"026_CubeSky.fx");
	sky = new CubeSky(skyShader, L"GrassCube1024.dds");

	terrainShader = new Shader(L"028_Terrain.fx");
	terrain = new Terrain(terrainShader, L"HeightMap256.png");	
}

void TestRect::Ready()
{
	terrain->Ready();
	terrain->BaseTexture(L"Dirt.png");
}

void TestRect::Destroy()
{
	
}

void TestRect::Update()
{
	sky->Update();
	terrain->Update();
}

void TestRect::PreRender()
{

}

void TestRect::Render()
{
	sky->Render();
	terrain->Render();
}
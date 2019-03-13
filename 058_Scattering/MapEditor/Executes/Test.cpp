#include "stdafx.h"
#include "Test.h"
#include "Environment/Sky.h"
#include "Environment/Terrain.h"

void Test::Initialize()
{
	sky = new Sky();

	
	Terrain::InitializeInfo terrainInfo;
	terrainInfo.shader = new Shader(L"057_Terrain_Tesselation.fx");
	//terrainInfo.heightMap = L"Terrain.png";
	//terrainInfo.HeightRatio = 100;
	terrainInfo.heightMap = L"HeightMap256.png";
	terrainInfo.HeightRatio = 20;
		
	terrain = new Terrain(terrainInfo);
}

void Test::Ready()
{
	sky->Ready();

	terrain->Ready();
	terrain->BaseTexture(L"Dirt.png");
}

void Test::Destroy()
{
	
}

void Test::Update()
{
	sky->Update();
	terrain->Update();
}

void Test::PreRender()
{

}

void Test::Render()
{
	sky->Render();
	terrain->Render();
}

void Test::PostRender()
{
	sky->PostRender();
}

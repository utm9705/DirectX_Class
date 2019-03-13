#include "stdafx.h"
#include "Test.h"
#include "Environment/CubeSky.h"
#include "Environment/Terrain.h"

void Test::Initialize()
{
	cubeSky = new CubeSky
	(
		new Shader(L"026_CubeSky.fx"),
		L"GrassCube1024.dds"
	);

	
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
	terrain->Ready();
	terrain->BaseTexture(L"Dirt.png");
}

void Test::Destroy()
{
	
}

void Test::Update()
{
	cubeSky->Update();
	terrain->Update();
}

void Test::PreRender()
{

}

void Test::Render()
{
	cubeSky->Render();
	terrain->Render();
}
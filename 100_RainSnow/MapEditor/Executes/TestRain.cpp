#include "stdafx.h"
#include "TestRain.h"


#include "Environment/Terrain.h"
#include "Environment\Rain.h"
#include "Environment\Snow.h"
#include "Environment/RainInst.h"
#include "Environment\SnowInst.h"

void TestRain::Initialize()
{

	
	Terrain::InitializeInfo terrainInfo;
	terrainInfo.shader = new Shader(L"057_Terrain_Tesselation.fx");
	//terrainInfo.heightMap = L"Terrain.png";
	//terrainInfo.HeightRatio = 100;
	terrainInfo.heightMap = L"Terrain/HeightMap256.png";
	terrainInfo.HeightRatio = 20;
		
	terrain = new Terrain(terrainInfo);



}

void TestRain::Ready()
{


	terrain->Ready();
	terrain->BaseTexture(L"Terrain/Dirt.png");


	////Rain
	//{
	//	Rain::InitDesc desc;
	//	desc.Size = D3DXVECTOR3(100, 100, 100);
	//	desc.Count = 500;

	//	rain = new Rain(desc);
	//}

	////Snow
	//{
	//	Snow::InitDesc desc;
	//	desc.Size = D3DXVECTOR3(100, 100, 100);
	//	desc.Count = 2000;

	//	snow = new Snow(desc);
	//}





	for (int i = 0; i < 10; i++)
	{

		//RainInst
		RainInst::InitDesc initDesc;
		initDesc.Size = D3DXVECTOR3(200, 20 * i, 1000);
		initDesc.InstCount = 200; // Á¤Á¡ °¹¼ö

		RainInst* rainInst = new RainInst(initDesc);
		vecRain.push_back(rainInst);
	}



	//RainInst
	RainInst::InitDesc initDesc;
	initDesc.Size = D3DXVECTOR3(200, 200, 1000);
	initDesc.InstCount = 1000; // Á¤Á¡ °¹¼ö
	initDesc.Count = 1;
	rainInst = new RainInst(initDesc);


	//SnowInst
	{
		SnowInst::InitDesc desc;
		desc.Size = D3DXVECTOR3(200, 200, 1000);
		desc.InstCount = 2000; // Á¤Á¡ °¹¼ö
		
		snowInst = new SnowInst(desc);
	}



}

void TestRain::Destroy()
{
	
}

void TestRain::Update()
{

	terrain->Update();

	//rain->Update();
	//snow->Update();

	//for (RainInst* rain : vecRain)
	//	rain->Update();

	//rainInst->Update();

	snowInst->Update();
}

void TestRain::PreRender()
{

}

void TestRain::Render()
{

	terrain->Render();

	//rain->Render();
	//snow->Render();

	//for (RainInst* rain : vecRain)
	//	rain->Render();

	//rainInst->Render();
	snowInst->Render();
}

void TestRain::PostRender()
{

}

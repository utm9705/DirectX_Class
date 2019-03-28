#include "stdafx.h"
#include "TestDepth.h"
#include "Environment/CubeSky.h"
#include "Objects/Shadow.h"

void TestDepth::Initialize()
{
	shader = new Shader(L"061_Shadow.fx");
}

void TestDepth::Ready()
{
	sky = new CubeSky(L"Environment/SnowCube1024.dds");
	
	floor = new Texture(L"Floor.png");
	stone = new Texture(L"Stones.png");
	brick = new Texture(L"Bricks.png");
	wall = new Texture(L"Wall.png");


	cube = new MeshCube(shader);
	cube->Position(0, 5.0f, 0);
	cube->Scale(20.0f, 10.0f, 20.0);

	grid = new MeshGrid(shader);
	grid->Position(0, 0, 0);
	grid->Scale(20, 1, 20);


	for (UINT i = 0; i < 5; i++)
	{
		cylinder[i * 2] = new MeshCylinder(shader, 0.5f, 3.0f, 20, 20);
		cylinder[i * 2]->Position(-30, 6.0f, -15.0f + (float)i * 15.0f);
		cylinder[i * 2]->Scale(5, 5, 5);

		cylinder[i * 2 + 1] = new MeshCylinder(shader, 0.5f, 3.0f, 20, 20);
		cylinder[i * 2 + 1]->Position(30, 6.0f, -15.0f + (float)i * 15.0f);
		cylinder[i * 2 + 1]->Scale(5, 5, 5);


		sphere[i * 2] = new MeshSphere(shader, 0.5f, 20, 20);
		sphere[i * 2]->Position(-30.0f, 16.0f, -15.0f + i * 15.0f);
		sphere[i * 2]->Scale(5, 5, 5);

		sphere[i * 2 + 1] = new MeshSphere(shader, 0.5f, 20, 20);
		sphere[i * 2 + 1]->Position(30.0f, 16.0f, -15.0f + i * 15.0f);
		sphere[i * 2 + 1]->Scale(5, 5, 5);
	}


	shadow = new Shadow(shader, 2048, 2048);
	shadow->Ready();
}

void TestDepth::Destroy()
{
	
}

void TestDepth::Update()
{
	sky->Update();
	shadow->Update();

	for (UINT i = 0; i < 10; i++)
	{
		sphere[i]->Update();
		cylinder[i]->Update();
	}
	
	cube->Update();
	grid->Update();
}

void TestDepth::PreRender()
{
	shadow->PreRender();
	
	for (UINT i = 0; i < 10; i++)
	{
		sphere[i]->Pass(0);
		sphere[i]->Render();

		cylinder[i]->Pass(0);
		cylinder[i]->Render();
	}

	cube->Pass(0);
	cube->Render();

	grid->Pass(0);
	grid->Render();
}

void TestDepth::Render()
{
	sky->Render();
	shadow->Render();

	shader->AsShaderResource("DiffuseMap")->SetResource(wall->SRV());
	for (UINT i = 0; i < 10; i++)
	{
		sphere[i]->Pass(1);
		sphere[i]->Render();
	}

	shader->AsShaderResource("DiffuseMap")->SetResource(brick->SRV());
	for (UINT i = 0; i < 10; i++)
	{
		cylinder[i]->Pass(1);
		cylinder[i]->Render();
	}

	shader->AsShaderResource("DiffuseMap")->SetResource(stone->SRV());
	cube->Pass(1);
	cube->Render();


	shader->AsShaderResource("DiffuseMap")->SetResource(floor->SRV());
	grid->Pass(1);
	grid->Render();
}

void TestDepth::PostRender()
{
	shadow->PostRender();
}

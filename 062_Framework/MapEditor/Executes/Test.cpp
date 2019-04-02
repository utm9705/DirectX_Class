#include "stdafx.h"
#include "Test.h"
#include "Environment/CubeSky.h"

void Test::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(11, 0);
	Context::Get()->GetCamera()->Position(0, 31, -82);
}

void Test::Ready()
{
	//Skydome
	{
		sky = new CubeSky(L"Environment/GrassCube1024.dds");
	}

	//Create Mesh
	{
		specular = D3DXCOLOR(1, 1, 1, 20);

		shader = new Shader(L"062_Lighting.fx");

		floor = new Material(shader);
		floor->DiffuseMap(L"Floor.png");
		floor->SpecularMap(L"Floor_Specular.png");
		floor->NormalMap(L"Floor_Normal.png");

		stone = new Material(shader);
		stone->Diffuse(0.8f, 0.7f, 0.7f);
		stone->DiffuseMap(L"Stones.png");
		stone->SpecularMap(L"Stones_Specular.png");
		stone->NormalMap(L"Stones_Normal.png");

		brick = new Material(shader);
		brick->Specular(1, 0, 0, 20);
		brick->DiffuseMap(L"Bricks.png");
		brick->SpecularMap(L"Bricks_Specular.png");
		brick->NormalMap(L"Bricks_Normal.png");

		wall = new Material(shader);
		wall->Specular(1, 0, 0, 20);
		wall->DiffuseMap(L"Wall.png");
		wall->SpecularMap(L"Wall_Specular.png");
		wall->NormalMap(L"Wall_Normal.png");


		cube = new MeshCube(shader);
		cube->GetTransform()->Position(0, 5.0f, 0);
		cube->GetTransform()->Scale(20.0f, 10.0f, 20.0);

		grid = new MeshGrid(shader, 4, 4);
		grid->GetTransform()->Position(0, 0, 0);
		grid->GetTransform()->Scale(20, 1, 20);


		for (UINT i = 0; i < 5; i++)
		{
			cylinder[i * 2] = new MeshCylinder(shader, 0.5f, 3.0f, 20, 20);
			cylinder[i * 2]->GetTransform()->Position(-30, 6.0f, -15.0f + (float)i * 15.0f);
			cylinder[i * 2]->GetTransform()->Scale(5, 5, 5);

			cylinder[i * 2 + 1] = new MeshCylinder(shader, 0.5f, 3.0f, 20, 20);
			cylinder[i * 2 + 1]->GetTransform()->Position(30, 6.0f, -15.0f + (float)i * 15.0f);
			cylinder[i * 2 + 1]->GetTransform()->Scale(5, 5, 5);


			sphere[i * 2] = new MeshSphere(shader, 0.5f, 20, 20);
			sphere[i * 2]->GetTransform()->Position(-30.0f, 15.5f, -15.0f + i * 15.0f);
			sphere[i * 2]->GetTransform()->Scale(5, 5, 5);

			sphere[i * 2 + 1] = new MeshSphere(shader, 0.5f, 20, 20);
			sphere[i * 2 + 1]->GetTransform()->Position(30.0f, 15.5f, -15.0f + i * 15.0f);
			sphere[i * 2 + 1]->GetTransform()->Scale(5, 5, 5);
		}
	}

	//PointLights
	{
		PointLight light = { Color(1, 0, 0, 1), Vector3(20, 0, 0), 20, 1 };
		Context::Get()->AddPointLight(light);

		light = { Color(1, 1, 0, 1), Vector3(-26, 0, 0), 10, 3 };
		Context::Get()->AddPointLight(light);
	}


	//SpotLights
	{
		SpotLight light = { Color(0, 0, 1, 1), Vector3(0, 20, 0), 0.3f, 1.0f, Vector3(0, -1, 0)};
		Context::Get()->AddSpotLight(light);
	}
}

void Test::Destroy()
{
	
}

void Test::Update()
{
	sky->Update();

	for (UINT i = 0; i < 10; i++)
	{
		sphere[i]->Update();
		cylinder[i]->Update();
	}

	cube->Update();
	grid->Update();


	//Specular
	{
		ImGui::ColorEdit3("Specular", (float *)&specular);
		ImGui::SliderFloat("Shininess", &specular.a, 1.0f, 30.0f);

		floor->Specular(specular);
		stone->Specular(specular);
		wall->Specular(specular);
		brick->Specular(specular);
	}

	//PointLight
	{
		
	}
}

void Test::PreRender()
{
	
}

void Test::Render()
{
	sky->Render();

	wall->Render();
	for (UINT i = 0; i < 10; i++)
		sphere[i]->Render();


	brick->Render();
	for (UINT i = 0; i < 10; i++)
		cylinder[i]->Render();


	stone->Render();
	cube->Render();

	floor->Render();
	grid->Render();
}

void Test::PostRender()
{
	
}

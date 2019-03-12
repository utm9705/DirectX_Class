#include "stdafx.h"
#include "TestRect.h"
#include "Environment/CubeSky.h"
#include "Environment/Terrain.h"

void TestRect::Initialize()
{
	cubeShader = new Shader(L"057_NormalMap.fx");
}

void TestRect::Ready()
{
	cubeMaterial = new Material(cubeShader);
	cubeMaterial->DiffuseMap("Brick.jpg");
	cubeMaterial->NormalMap("Brick_Normal.jpg");
	cubeMaterial->SpecularMap("Brick_Specular.jpg");
	cubeMaterial->Specular(0.3f, 0.3f, 0.3f, 1);

	cube = new MeshSphere(cubeShader, 5, 100, 100);
}

void TestRect::Destroy()
{
	SAFE_DELETE(cube);
	SAFE_DELETE(cubeShader);
}

void TestRect::Update()
{
	static UINT VisibleBump = 0;
	ImGui::SliderInt("Visible", (int *)&VisibleBump, 0, 2);
	cubeShader->AsScalar("VisibleBump")->SetInt(VisibleBump);

	static float Shininess = 5.0f;
	ImGui::SliderFloat("Shininess", &Shininess, 0.1f, 40.0f);
	cubeMaterial->Shininess(Shininess);

	cube->Update();
}

void TestRect::PreRender()
{

}

void TestRect::Render()
{
	cubeMaterial->Render();
	cube->Render();
}
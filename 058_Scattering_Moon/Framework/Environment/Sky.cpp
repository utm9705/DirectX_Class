#include "Framework.h"
#include "Sky.h"
#include "Scattering.h"
#include "SkyDome.h"
#include "Cloud.h"
#include "Moon.h"

Sky::Sky()
	: Renderer()
	, realTime(false), theta(0), phi(0), timeFactor(0.0f)
	, prevTheta(0), prevPhi(0)
{
	shader = new Shader(L"_Project/058_Scattering.fx");
	SetShader(shader);

	scattering = new Scattering(shader);
	skyDome = new SkyDome(shader);
	cloud = new Cloud(shader);
	moon = new Moon(shader);

	targetDesc.InvWaveLength.x = 1.0f / powf(targetDesc.WaveLength.x, 4.0f);
	targetDesc.InvWaveLength.y = 1.0f / powf(targetDesc.WaveLength.y, 4.0f);
	targetDesc.InvWaveLength.z = 1.0f / powf(targetDesc.WaveLength.z, 4.0f);

	targetDesc.WaveLengthMie.x = powf(targetDesc.WaveLength.x, -0.84f);
	targetDesc.WaveLengthMie.y = powf(targetDesc.WaveLength.y, -0.84f);
	targetDesc.WaveLengthMie.z = powf(targetDesc.WaveLength.z, -0.84f);

	targetBuffer = new CBuffer(&targetDesc, sizeof(TargetDesc));
	sTargetBuffer = shader->AsConstantBuffer("CB_Target");

	nightBuffer = new CBuffer(&nightDesc, sizeof(NightDesc));
	sNightBuffer = shader->AsConstantBuffer("CB_Night");

	cloudBuffer = new CBuffer(&cloudDesc, sizeof(CloudDesc));
	sCloudBuffer = shader->AsConstantBuffer("CB_Cloud");


	sStarMap = shader->AsShaderResource("StarMap");
	sRayleighMap = shader->AsShaderResource("RayleighMap");
	sMieMap = shader->AsShaderResource("MieMap");
	sCloudMap = shader->AsShaderResource("CloudMap");

	// 달

	sMoonMap = shader->AsShaderResource("MoonMap");
}

Sky::~Sky()
{

	SAFE_DELETE(scattering);
	SAFE_DELETE(skyDome);
	SAFE_DELETE(cloud);
	//SAFE_DELETE(moon);

	SAFE_DELETE(targetBuffer);
	SAFE_DELETE(shader);
}

void Sky::Ready()
{
	starMap = new Texture(L"Starfield.png");
	sStarMap->SetResource(starMap->SRV());

	moonField = new Texture(L"Box.png");
	moonGlowField = new Texture(L"Box.png");


	scattering->Ready();
	skyDome->Ready();
	moon->Ready();
	cloud->Ready();
	
}

void Sky::Update()
{
	Renderer::Update();

	// 움직임
	{
		theta += Time::Delta() * timeFactor;

		if (theta > Math::PI * 2.0f) theta -= Math::PI * 2.0f;

		Context::Get()->LightDirection() = GetDirection();
	}


	scattering->Update();
	skyDome->Update();
	moon->Update();
	cloud->Update();
}

void Sky::PreRender()
{
	if (prevTheta == theta && prevPhi == phi)
		return;

	targetBuffer->Apply();
	sTargetBuffer->SetConstantBuffer(targetBuffer->Buffer());

	Renderer::Render();
	scattering->PreRender();
}

void Sky::Render()
{
	D3DXVECTOR3 position; // 카메라 포지션
	Context::Get()->GetCamera()->Position(&position);

	//SkyDome
	{


		position.y -= 0.2f;
		Position(position);


		//빛의 높이 = 해의 높이
		sRayleighMap->SetResource(scattering->Rayleigh()->GetSRV());
		sMieMap->SetResource(scattering->Mie()->GetSRV());

		Scale(1, 1, 1);
		Rotation(0, 0, 0);

		Renderer::Render();
		skyDome->Render();
	}

	//Moon
	{
		D3DXMATRIX sc, rX, rY, dist, cam, matF;

		D3DXMatrixScaling(&sc, 50, 50, 1);
		D3DXMatrixRotationX(&rX, -theta - (Math::PI * 0.5f));
		D3DXMatrixRotationY(&rY, phi - (Math::PI * 0.5f));

		D3DXMatrixTranslation(&cam, position.x, position.y, position.z);

		if (theta < Math::PI * 0.5f || theta > Math::PI * 1.5f)
			nightDesc.MoonAlpha = (float)abs(sinf(theta + (float)Math::PI / 2.0f));
		else
			nightDesc.MoonAlpha = 0.0f;

		// moon
		{
			D3DXMatrixTranslation(&dist,
				Context::Get()->LightDirection().x * 815.0f,
				Context::Get()->LightDirection().y * 815.0f,
				Context::Get()->LightDirection().z * 815.0f);
			matF = sc*rX*rY*dist*cam;

			World(matF);


			sMoonMap->SetResource(moonField->SRV());

			Renderer::Render();
			moon->Render();
		}


		//Moon Glow
		{

			D3DXMatrixScaling(&sc, 150, 150, 1);
			D3DXMatrixTranslation(&dist,
				Context::Get()->LightDirection().x * 805.0f,
				Context::Get()->LightDirection().y * 805.0f,
				Context::Get()->LightDirection().z * 805.0f);
			matF = sc*rX*rY*dist*cam;

			World(matF);

			sMoonMap->SetResource(moonGlowField->SRV());
			Renderer::Render();
			moon->Render();
		}

	}



	//Cloud
	{
		nightDesc.MoonAlpha = Time::Get()->Running() / 8.0f;

		nightDesc.StarIntensity = Context::Get()->LightDirection().y;

		nightBuffer->Apply();
		sNightBuffer->SetConstantBuffer(nightBuffer->Buffer());


		cloudBuffer->Apply();
		sCloudBuffer->SetConstantBuffer(cloudBuffer->Buffer());


		Scale(90, 90, 1);
		Rotation(-Math::PI * 0.5f, 0, 0);
		Position(position.x, position.y + 2, position.z);

		sCloudMap->SetResource(cloud->Srv());

		Renderer::Render();
		cloud->Render();
	}


	ImGui::SliderFloat("TimeFactor", &timeFactor, 0.0f, 5.0f);
	ImGui::SliderFloat("Theta", &theta, 0.0f, Math::PI * 2.0f, "%.5f");
	ImGui::SliderFloat("Phi", &phi, 0.0f, Math::PI * 2.0f, "%.5f");
}

void Sky::PostRender()
{
	scattering->PostRender();
}

D3DXVECTOR3 Sky::GetDirection()
{
	float y = (float)cosf(theta);
	float x = (float)(sinf(theta));
	float z = (float)(sinf(Math::ToRadian(phi)));

	return D3DXVECTOR3(x, y, z);
}

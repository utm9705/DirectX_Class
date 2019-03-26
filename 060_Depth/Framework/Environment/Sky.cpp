#include "Framework.h"
#include "Sky.h"
#include "Scattering.h"
#include "SkyDome.h"
#include "Cloud.h"
#include "Moon.h"

Sky::Sky()
	: Renderer()
	, realTime(false), theta(0), prevTheta(0), phi(0), prevPhi(0)
	, timeFactor(0.25f)
{
	shader = new Shader(L"058_Scattering.fx");
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
	sMoonMap = shader->AsShaderResource("MoonMap");
}

Sky::~Sky()
{
	SAFE_DELETE(cloud);
	SAFE_DELETE(scattering);
	SAFE_DELETE(skyDome);
	SAFE_DELETE(moon);

	SAFE_DELETE(targetBuffer);
	SAFE_DELETE(shader);
}

void Sky::Ready()
{
	starMap = new Texture(L"Environment/Starfield.png");
	sStarMap->SetResource(starMap->SRV());

	moonMap = new Texture(L"Environment/Moon.png");
	moonGlowMap = new Texture(L"Environment/MoonGlow.png");


	scattering->Ready();
	skyDome->Ready();
	cloud->Ready();
	moon->Ready();
}

void Sky::Update()
{
	Renderer::Update();

	//Angle
	{
		theta += Time::Delta() * timeFactor;
		
		if (theta > Math::PI * 2.0f) 
			theta -= Math::PI * 2.0f;


		float x = sinf(theta);
		float y = cosf(theta);
		float z = sinf(Math::ToRadian(phi));

		Context::Get()->LightDirection() = D3DXVECTOR3(x, y, z);
	}
	
	scattering->Update();
	skyDome->Update();
	cloud->Update();
	moon->Update();
}

void Sky::PreRender()
{
	if (prevTheta == theta)
		return;

	prevTheta = theta;

	targetBuffer->Apply();
	sTargetBuffer->SetConstantBuffer(targetBuffer->Buffer());

	Renderer::Render();
	scattering->PreRender();
}

void Sky::Render()
{
	D3DXVECTOR3 position;
	Context::Get()->GetCamera()->Position(&position);

	//SkyDome
	{
		position.y -= 0.2f;

		Position(position.x, position.y, position.z);
		Scale(1, 1, 1);
		Rotation(0, 0, 0);

		sRayleighMap->SetResource(scattering->Rayleigh()->GetRT_SRV());
		sMieMap->SetResource(scattering->Mie()->GetRT_SRV());

		Renderer::Render();
		skyDome->Render();
	}

	//Moon
	{
		nightDesc.MoonAlpha = moon->GetMoonAlpha(theta);

		World(moon->GetTransform(theta));
		sMoonMap->SetResource(moonMap->SRV());
		
		Renderer::Render();
		moon->Render();
	}

	//MoonGlow
	{
		World(moon->GetGlowTransform(theta));
		sMoonMap->SetResource(moonGlowMap->SRV());

		Renderer::Render();
		moon->Render();
	}

	//Cloud
	{
		nightDesc.MoonAlpha = Time::Get()->Running() / 8.0f;
		nightDesc.StarIntensity = Context::Get()->LightDirection().y;
		
		nightBuffer->Apply();
		sNightBuffer->SetConstantBuffer(nightBuffer->Buffer());

		cloudBuffer->Apply();
		sCloudBuffer->SetConstantBuffer(cloudBuffer->Buffer());

		sCloudMap->SetResource(cloud->SRV());

		Scale(500, 500, 1);
		Rotation(-Math::PI * 0.5f, 0, 0);
		Position(position.x + 95, position.y + 30.0f, position.z + 300.0f);

		Renderer::Render();
		cloud->Render();
	}

	ImGui::SliderFloat("Tiles", &cloudDesc.Tiles, 0.1f, 10.0f);
	ImGui::SliderFloat("Cover", &cloudDesc.Cover, 0.001f, 1.0f);
	ImGui::SliderFloat("Sharpness", &cloudDesc.Sharpness, 0.001f, 1.0f);
}

void Sky::PostRender()
{
	scattering->PostRender();
}
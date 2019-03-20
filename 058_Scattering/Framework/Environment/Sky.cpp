#include "Framework.h"
#include "Sky.h"
#include "Scattering.h"
#include "SkyDome.h"
#include "Cloud.h"

Sky::Sky()
	: Renderer()
	, realTime(false), theta(0), phi(0)
{
	shader = new Shader(L"058_Scattering.fx");
	SetShader(shader);

	scattering = new Scattering(shader);
	skyDome = new SkyDome(shader);
	cloud = new Cloud(shader);

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
}

Sky::~Sky()
{
	SAFE_DELETE(scattering);
	SAFE_DELETE(skyDome);

	SAFE_DELETE(targetBuffer);
	SAFE_DELETE(shader);
}

void Sky::Ready()
{
	starMap = new Texture(L"StarField.png");
	sStarMap->SetResource(starMap->SRV());

	scattering->Ready();
	skyDome->Ready();
	cloud->Ready();
}

void Sky::Update()
{
	Renderer::Update();

	scattering->Update();
	skyDome->Update();
	cloud->Update();
}

void Sky::PreRender()
{
	targetBuffer->Apply();
	sTargetBuffer->SetConstantBuffer(targetBuffer->Buffer());

	Renderer::Render();
	scattering->PreRender();
}

void Sky::Render()
{
	D3DXVECTOR3 position;
	Context::Get()->GetCamera()->Position(&position);

	//skyDome
	{
		Position(position.x, position.y - 0.2f, position.z);

		Scale(1, 1, 1);
		Rotation(0, 0, 0);
		//빛의 높이 = 해의 높이
		sRayleighMap->SetResource(scattering->Rayleigh()->GetSRV());
		sMieMap->SetResource(scattering->Mie()->GetSRV());

		Renderer::Render();
		skyDome->Render();
	}

	//cloud
	{
		nightDesc.MoonAlpha = Time::Get()->Running() / 8.0f;
		nightDesc.StarIntensity = Context::Get()->LightDirection().y;
		nightBuffer->Apply();
		sNightBuffer->SetConstantBuffer(nightBuffer->Buffer());

		cloudBuffer->Apply();
		sCloudBuffer->SetConstantBuffer(cloudBuffer->Buffer());

		sCloudMap->SetResource(cloud->SRV());

		Scale(10, 10, 1);
		Rotation(-Math::PI * 0.5f, 0, 0);
		Position(position.x, position.y + 1, position.z);

		Renderer::Render();
		cloud->Render();
	}
}

void Sky::PostRender()
{
	scattering->PostRender();
}
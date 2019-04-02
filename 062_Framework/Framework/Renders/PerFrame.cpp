#include "Framework.h"
#include "PerFrame.h"

PerFrame::PerFrame(Shader * shader)
	: shader(shader)
{
	buffer = new ConstantBuffer(&bufferDesc, sizeof(BufferDesc));
	lightBuffer = new ConstantBuffer(&lightDesc, sizeof(LightDesc));
	pointLightBuffer = new ConstantBuffer(&pointLightDesc, sizeof(PointLightDesc));
	spotLightBuffer = new ConstantBuffer(&spotLightDesc, sizeof(SpotLightDesc));

	sBuffer = shader->AsConstantBuffer("CB_PerFrame");
	sLightBuffer = shader->AsConstantBuffer("CB_Light");
	sPointLightBuffer = shader->AsConstantBuffer("CB_PointLight");
	sSpotLightBuffer = shader->AsConstantBuffer("CB_SpotLight");


	D3DXMatrixIdentity(&bufferDesc.View);
	D3DXMatrixIdentity(&bufferDesc.ViewInverse);
	D3DXMatrixIdentity(&bufferDesc.Projection);
	D3DXMatrixIdentity(&bufferDesc.ViewProjection);

	lightDesc.Ambient = D3DXCOLOR(0, 0, 0, 1);
	lightDesc.Specular = D3DXCOLOR(1, 1, 1, 1);
	lightDesc.Direction = D3DXVECTOR3(-1, -1, 1);
	lightDesc.Position = D3DXVECTOR3(0, 0, 0);
}

PerFrame::~PerFrame()
{
	SAFE_DELETE(buffer);
	SAFE_DELETE(lightBuffer);
	SAFE_DELETE(pointLightBuffer);
	SAFE_DELETE(spotLightBuffer);
}

void PerFrame::Update()
{
	bufferDesc.View = Context::Get()->View();
	D3DXMatrixInverse(&bufferDesc.ViewInverse, NULL, &bufferDesc.View);

	bufferDesc.Projection = Context::Get()->Projection();
	bufferDesc.ViewProjection = bufferDesc.View * bufferDesc.Projection;

	lightDesc.Ambient = Context::Get()->LightAmbient();
	lightDesc.Specular = Context::Get()->LightSpecular();
	lightDesc.Position = Context::Get()->LightPosition();
	lightDesc.Direction = Context::Get()->LightDirection();

	bufferDesc.Time = Time::Get()->Running();

	pointLightDesc.Count = Context::Get()->PointLights(pointLightDesc.Lights);
	spotLightDesc.Count = Context::Get()->SpotLights(spotLightDesc.Lights);
}

void PerFrame::Render()
{
	buffer->Apply();
	sBuffer->SetConstantBuffer(buffer->Buffer());

	if (sLightBuffer != NULL)
	{
		lightBuffer->Apply();
		sLightBuffer->SetConstantBuffer(lightBuffer->Buffer());

		pointLightBuffer->Apply();
		sPointLightBuffer->SetConstantBuffer(pointLightBuffer->Buffer());

		spotLightBuffer->Apply();
		sSpotLightBuffer->SetConstantBuffer(spotLightBuffer->Buffer());
	}
}

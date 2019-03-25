#include "Framework.h"
#include "Moon.h"

Moon::Moon(Shader * shader)
	: shader(shader)
	, distance(815), glowDistance(805)
{

}

Moon::~Moon()
{

}

void Moon::Ready()
{
	VertexTexture vertices[6];

	vertices[0].Position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	vertices[1].Position = D3DXVECTOR3(-1.0f, +1.0f, 0.0f);
	vertices[2].Position = D3DXVECTOR3(+1.0f, -1.0f, 0.0f);
	vertices[3].Position = D3DXVECTOR3(+1.0f, -1.0f, 0.0f);
	vertices[4].Position = D3DXVECTOR3(-1.0f, +1.0f, 0.0f);
	vertices[5].Position = D3DXVECTOR3(+1.0f, +1.0f, 0.0f);

	vertices[0].Uv = D3DXVECTOR2(0, 1);
	vertices[1].Uv = D3DXVECTOR2(0, 0);
	vertices[2].Uv = D3DXVECTOR2(1, 1);
	vertices[3].Uv = D3DXVECTOR2(1, 1);
	vertices[4].Uv = D3DXVECTOR2(0, 0);
	vertices[5].Uv = D3DXVECTOR2(1, 0);


	Renderer::CreateVertexBuffer(vertices, 6, sizeof(VertexTexture), 0, &vertexBuffer);
}

void Moon::Update()
{
}

void Moon::Render()
{
	UINT stride = sizeof(VertexTexture);
	UINT offset = 0;

	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	shader->Draw(0, 3, 6);
}

float Moon::GetMoonAlpha(float theta)
{
	if (theta < Math::PI * 0.5f || theta > Math::PI * 1.5f)
		return fabsf(sinf(theta + Math::PI / 2.0f));

	return 0.0f;
}

D3DXMATRIX Moon::GetTransform(float theta)
{
	D3DXVECTOR3 position;
	Context::Get()->GetCamera()->Position(&position);

	D3DXMATRIX S, R, T, D;
	D3DXMatrixScaling(&S, 50, 50, 1);
	D3DXMatrixRotationYawPitchRoll(&R, Math::PI * 0.5f, -theta - (Math::PI * 0.5f), 0);
	D3DXMatrixTranslation(&T, position.z, position.y, position.z);

	D3DXVECTOR3 direction = Context::Get()->LightDirection();
	D3DXMatrixTranslation(&D
		, direction.x * distance
		, direction.y * distance
		, direction.z * distance
	);

	return S * R * T * D;
}

D3DXMATRIX Moon::GetGlowTransform(float theta)
{
	D3DXVECTOR3 position;
	Context::Get()->GetCamera()->Position(&position);

	D3DXMATRIX S, R, T, D;
	D3DXMatrixScaling(&S, 150, 150, 1);
	D3DXMatrixRotationYawPitchRoll(&R, Math::PI * 0.5f, -theta - (Math::PI * 0.5f), 0);
	D3DXMatrixTranslation(&T, position.z, position.y, position.z);

	D3DXVECTOR3 direction = Context::Get()->LightDirection();
	D3DXMatrixTranslation(&D
		, direction.x * glowDistance
		, direction.y * glowDistance
		, direction.z * glowDistance
	);

	return S * R * T * D;
}



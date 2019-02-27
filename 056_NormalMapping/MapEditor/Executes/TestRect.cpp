#include "stdafx.h"
#include "TestRect.h"

void TestRect::Initialize()
{
	shader = new Shader(L"056_NormalMap.fx");

	VertexTexture vertices[4];
	vertices[0].Position = D3DXVECTOR3(-0.5f, -0.5f, 0);
	vertices[1].Position = D3DXVECTOR3(-0.5f, +0.5f, 0);
	vertices[2].Position = D3DXVECTOR3(+0.5f, -0.5f, 0);
	vertices[3].Position = D3DXVECTOR3(+0.5f, -0.5f, 0);
	vertices[4].Position = D3DXVECTOR3(-0.5f, +0.5f, 0);
	vertices[5].Position = D3DXVECTOR3(+0.5f, +0.5f, 0);

	vertices[0].Uv = D3DXVECTOR2(0, 1);
	vertices[1].Uv = D3DXVECTOR2(0, 0);
	vertices[2].Uv = D3DXVECTOR2(1, 1);
	vertices[3].Uv = D3DXVECTOR2(1, 1);
	vertices[4].Uv = D3DXVECTOR2(0, 0);
	vertices[5].Uv = D3DXVECTOR2(1, 0);

	CsResource::CreateRawBuffer(sizeof(VertexTexture) * 6, vertices, &vertexBuffer);
}

void TestRect::Ready()
{
	
}

void TestRect::Destroy()
{
	
}

void TestRect::Update()
{
	D3DXMATRIX W;
	D3DXMatrixIdentity(&W);

	shader->AsMatrix("World")->SetMatrix(W);
}

void TestRect::PreRender()
{

}

void TestRect::Render()
{
	UINT stride = sizeof(VertexTexture);
	UINT offset = 0;

	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	shader->Draw(0, 0, 6);
}
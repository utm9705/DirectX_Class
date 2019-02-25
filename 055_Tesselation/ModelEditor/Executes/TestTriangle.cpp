#include "stdafx.h"
#include "TestTriangle.h"
#include "Assimp/Loader.h"

void TestTriangle::Initialize()
{
	shader = new Shader(L"055_TessTriangle.fx");

	Vertex vertices[3];
	vertices[0].Position = D3DXVECTOR3(-1, -1, 0);
	vertices[1].Position = D3DXVECTOR3( 0,  1, 0);
	vertices[2].Position = D3DXVECTOR3( 1, -1, 0);

	CsResource::CreateRawBuffer(sizeof(Vertex) * 3, vertices, &vertexBuffer);
}

void TestTriangle::Ready()
{
	
}

void TestTriangle::Destroy()
{
	
}

void TestTriangle::Update()
{
	D3DXMATRIX W;
	D3DXMatrixIdentity(&W);

	shader->AsMatrix("World")->SetMatrix(W);
}

void TestTriangle::PreRender()
{

}

void TestTriangle::Render()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

	shader->Draw(0, 0, 3);
}
#include "stdafx.h"
#include "TestRectDistance.h"

void TestRectDistance::Initialize()
{
	shader = new Shader(L"055_TessRectDistance.fx");

	Vertex vertices[4];
	vertices[0].Position = D3DXVECTOR3(-1, -1, 0);
	vertices[1].Position = D3DXVECTOR3(-1,  1, 0);
	vertices[2].Position = D3DXVECTOR3( 1, -1, 0);
	vertices[3].Position = D3DXVECTOR3( 1,  1, 0);

	CsResource::CreateRawBuffer(sizeof(Vertex) * 4, vertices, &vertexBuffer);
}

void TestRectDistance::Ready()
{
	
}

void TestRectDistance::Destroy()
{
	
}

void TestRectDistance::Update()
{
	D3DXMATRIX W;
	D3DXMatrixIdentity(&W);

	shader->AsMatrix("World")->SetMatrix(W);


	static float ratio = 20;
	static UINT maxPiece = 1;

	ImGui::SliderFloat("Ratio", &ratio, 1, 10);
	ImGui::SliderInt("MaxPiece", (int *)&maxPiece, 1, 64);

	shader->AsScalar("Ratio")->SetFloat(ratio);
	shader->AsScalar("MaxPiece")->SetInt(maxPiece);
}

void TestRectDistance::PreRender()
{

}

void TestRectDistance::Render()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);

	shader->Draw(0, 0, 4);
}
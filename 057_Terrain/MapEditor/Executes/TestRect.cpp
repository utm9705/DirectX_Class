#include "stdafx.h"
#include "TestRect.h"

void TestRect::Initialize()
{
	shader = new Shader(L"055_TessRect.fx");

	Vertex vertices[4];
	vertices[0].Position = D3DXVECTOR3(-1, -1, 0);
	vertices[1].Position = D3DXVECTOR3(-1,  1, 0);
	vertices[2].Position = D3DXVECTOR3( 1, -1, 0);
	vertices[3].Position = D3DXVECTOR3( 1,  1, 0);

	CsResource::CreateRawBuffer(sizeof(Vertex) * 4, vertices, &vertexBuffer);
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


	static UINT amount = 1;
	static UINT inside = 1;

	ImGui::SliderInt("Amount", (int *)&amount, 1, 64);
	ImGui::SliderInt("Inside", (int *)&inside, 1, 64);

	shader->AsScalar("TsAmount")->SetInt(amount);
	shader->AsScalar("TsAmountInside")->SetInt(inside);
}

void TestRect::PreRender()
{

}

void TestRect::Render()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);

	shader->Draw(0, 0, 4);
}
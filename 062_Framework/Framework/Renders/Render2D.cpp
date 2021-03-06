#include "framework.h"
#include "Render2D.h"

Render2D::Render2D(wstring shaderFile)
	: Renderer(L"026_Render2D.fx")
{
	//Create View Matrix
	{
		D3DXMatrixLookAtLH
		(
			&view, &D3DXVECTOR3(0, 0, -1), &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 1, 0)
		);
	}

	//Create Ortho Matrix
	{
		D3DDesc desc;
		D3D::GetDesc(&desc);

		D3DXMatrixOrthoOffCenterLH(&orthographic, 0, desc.Width, 0, desc.Height, -1, 1);
	}

	shader->AsMatrix("View2")->SetMatrix(view);
	shader->AsMatrix("Projection2")->SetMatrix(orthographic);

	sMap = shader->AsSRV("Map");
}

Render2D::~Render2D()
{
	
}

void Render2D::Ready()
{
	VertexTexture* vertices = new VertexTexture[6];

	vertices[0].Position = D3DXVECTOR3(-0.5f, -0.5f, 0.0f);
	vertices[1].Position = D3DXVECTOR3(-0.5f, +0.5f, 0.0f);
	vertices[2].Position = D3DXVECTOR3(+0.5f, -0.5f, 0.0f);
	vertices[3].Position = D3DXVECTOR3(+0.5f, -0.5f, 0.0f);
	vertices[4].Position = D3DXVECTOR3(-0.5f, +0.5f, 0.0f);
	vertices[5].Position = D3DXVECTOR3(+0.5f, +0.5f, 0.0f);

	vertices[0].Uv = D3DXVECTOR2(0, 1);
	vertices[1].Uv = D3DXVECTOR2(0, 0);
	vertices[2].Uv = D3DXVECTOR2(1, 1);
	vertices[3].Uv = D3DXVECTOR2(1, 1);
	vertices[4].Uv = D3DXVECTOR2(0, 0);
	vertices[5].Uv = D3DXVECTOR2(1, 0);


	vertexBuffer = new VertexBuffer(vertices, 6, sizeof(VertexTexture), 0);
	SAFE_DELETE_ARRAY(vertices);
}

void Render2D::Render()
{
	Renderer::Render();

	shader->Draw(0, Pass(), 6);
}

void Render2D::SRV(ID3D11ShaderResourceView * srv)
{
	sMap->SetResource(srv);
}
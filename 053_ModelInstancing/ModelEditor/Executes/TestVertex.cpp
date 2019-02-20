#include "stdafx.h"
#include "TestVertex.h"
#include "Assimp/Loader.h"

void TestVertex::Initialize()
{
	
}

void TestVertex::Ready()
{
	shader = new Shader(L"053_Texture.fx");

	vertices[0].Position = D3DXVECTOR3(-0.2f, -0.2f, 0.0f);
	vertices[1].Position = D3DXVECTOR3(-0.2f, +0.2f, 0.0f);
	vertices[2].Position = D3DXVECTOR3(+0.2f, -0.2f, 0.0f);
	vertices[3].Position = D3DXVECTOR3(+0.2f, -0.2f, 0.0f);
	vertices[4].Position = D3DXVECTOR3(-0.2f, +0.2f, 0.0f);
	vertices[5].Position = D3DXVECTOR3(+0.2f, +0.2f, 0.0f);
	
	CsResource::CreateRawBuffer(sizeof(Vertex) * 6, vertices, &vertexBuffer[0]);


	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
	textureDesc.Width = 128 * 4;
	textureDesc.Height = 128;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	textureDesc.Usage = D3D11_USAGE_DYNAMIC;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	HRESULT hr = D3D::GetDevice()->CreateTexture2D(&textureDesc, NULL, &worldData);
	assert(SUCCEEDED(hr));

	CsResource::CreateSrv(worldData, &worldSrv);
	shader->AsShaderResource("WorldData")->SetResource(worldSrv);


	D3DXMatrixTranslation(&world[0][0], 0.5f, 0.0f, 0.0f);
	D3DXMatrixTranslation(&world[0][1], 0.0f, 0.5f, 0.0f);
	D3DXMatrixTranslation(&world[0][2], 0.0f, 0.0f, 0.1f);
	D3DXMatrixTranslation(&world[1][0], -0.5f, 0.0f, 0.0f);
	D3DXMatrixTranslation(&world[1][1], 0.0f, -0.5f, 0.0f);
	D3DXMatrixTranslation(&world[1][2], 0.0f, 0.0f, 0.1f);

	D3D11_MAPPED_SUBRESOURCE subResource;
	D3D::GetDC()->Map(worldData, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		for (int i = 0; i < 2; i++)
		{
			void* p = ((D3DXMATRIX *)subResource.pData) + (i * 128);

			memcpy(p, world[i], sizeof(D3DXMATRIX) * 3);
		}
	}
	D3D::GetDC()->Unmap(worldData, 0);

	color[0] = D3DXCOLOR(1, 0, 0, 1);
	color[1] = D3DXCOLOR(0, 0, 1, 1);

	// CreateVertexBuffer
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(D3DXCOLOR) * 2;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = color;

		HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer[1]);
		assert(SUCCEEDED(hr));
	}
}

void TestVertex::Destroy()
{
}

void TestVertex::Update()
{
	ImGui::LabelText("FPS", "%d", (int)ImGui::GetIO().Framerate);
}

void TestVertex::PreRender()
{

}

void TestVertex::Render()
{
	UINT stride[2] = { sizeof(Vertex), sizeof(D3DXCOLOR) };
	UINT offset[2] = { 0, 0 };

	D3D::GetDC()->IASetVertexBuffers(0, 2, vertexBuffer, stride, offset);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	shader->DrawInstanced(0, 0, 6, 2);
	//shader->DrawIndexedInstanced(0, 0, 6, 1000);
}
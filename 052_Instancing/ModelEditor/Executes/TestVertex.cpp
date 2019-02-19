#include "stdafx.h"
#include "TestVertex.h"
#include "Assimp/Loader.h"

void TestVertex::Initialize()
{
	shader = new Shader(L"052_Instance.fx");

	vertices[0].Position = D3DXVECTOR3(-0.5f, -0.5f, 0.0f);
	vertices[1].Position = D3DXVECTOR3(-0.5f, +0.5f, 0.0f);
	vertices[2].Position = D3DXVECTOR3(+0.5f, -0.5f, 0.0f);
	vertices[3].Position = D3DXVECTOR3(+0.5f, +0.5f, 0.0f);

	vertices[0].Uv = D3DXVECTOR2(0, 1);
	vertices[1].Uv = D3DXVECTOR2(0, 0);
	vertices[2].Uv = D3DXVECTOR2(1, 1);
	vertices[3].Uv = D3DXVECTOR2(1, 0);

	// CreateVertexBuffer
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(VertexTexture) * 4;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = vertices;

		HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer[0]);
		assert(SUCCEEDED(hr));
	}

	
	for (int i = 0; i < 1000; i++)
	{
		D3DXVECTOR2 scale;
		scale.x = Math::Random(1.0f, 3.0f);
		scale.y = Math::Random(1.0f, 3.0f);

		D3DXVECTOR3 position;
		position.x = Math::Random(0.0f, 255.0f);
		position.z = Math::Random(0.0f, 255.0f);
		position.y = 0.0f;


		D3DXMATRIX S, T;
		D3DXMatrixScaling(&S, scale.x, scale.y, 1);
		D3DXMatrixTranslation(&T, position.x, position.y, position.z);

		world[i] = S * T;
	}

	// CreateVertexBuffer
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(D3DXMATRIX) * 1000;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = world;

		HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer[1]);
		assert(SUCCEEDED(hr));
	}

	UINT indices[6]{ 0, 1, 2, 2, 1, 3 };
	// CreateIndexBuffer
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(UINT) * 6;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = indices;

		HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &indexBuffer);
		assert(SUCCEEDED(hr));
	}
}

void TestVertex::Ready()
{
	vector<wstring> names;
	names.push_back(Textures + L"White.png");
	names.push_back(Textures + L"Red.png");
	names.push_back(Textures + L"Green.png");
	names.push_back(Textures + L"Blue.png");
	names.push_back(Textures + L"Magenta.png");
	names.push_back(Textures + L"Cyan.png");

	textures = new TextureArray(names);
	shader->AsShaderResource("MapArray")->SetResource(textures->SRV());
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
	/*for (int i = 0; i < 1000; i++)
	{
		shader->AsMatrix("World")->SetMatrix(world[i]);

		UINT stride[1] = { sizeof(VertexColor) };
		UINT offset[1] = { 0 };

		D3D::GetDC()->IASetVertexBuffers(0, 1, vertexBuffer, stride, offset);
		D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		shader->Draw(0, 0, 6);
	}*/

	UINT stride[2] = { sizeof(VertexTexture), sizeof(D3DXMATRIX) };
	UINT offset[2] = { 0, 0 };

	D3D::GetDC()->IASetVertexBuffers(0, 2, vertexBuffer, stride, offset);
	D3D::GetDC()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//shader->DrawInstanced(0, 0, 6, 1000);
	shader->DrawIndexedInstanced(0, 0, 6, 1000);
}
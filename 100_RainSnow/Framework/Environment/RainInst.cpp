#include "Framework.h"
#include "RainInst.h"

RainInst::RainInst(InitDesc & initDesc)
	: Renderer(), initDesc(initDesc)
{
	shader = new Shader(L"_Project/059_RainInst.fx");
	SetShader(shader);




	texture = new Texture(L"Rain.png");

	shader->AsShaderResource("Base")->SetResource(texture->SRV());

	buffer = new CBuffer(&bufferDesc, sizeof(BufferDesc));

	bufferDesc.Size = initDesc.Size;
	instCount = initDesc.InstCount;
	Count = initDesc.Count;

	sBuffer = shader->AsConstantBuffer("CB_Rain");
	sBuffer->SetConstantBuffer(buffer->Buffer());


	CreateData();

	// 인스턴스 생성
	if (vertexBuffer == NULL && indexBuffer == NULL)
	{
		CreateVertexBuffer(vertices, vertexCount, sizeof(VertexTextureNormal), 0);
		CreateIndexBuffer(indices, indexCount);
	}

}

RainInst::~RainInst()
{
	SAFE_DELETE(shader);
	SAFE_DELETE(buffer);

	SAFE_DELETE_ARRAY(vertices);
	SAFE_DELETE_ARRAY(indices);

	SAFE_DELETE(texture);
}

void RainInst::Update()
{
	Renderer::Update();



	D3DXVECTOR3 position;
	Context::Get()->GetCamera()->Position(&position);

	bufferDesc.Origin = position;

	//ImGui::SliderFloat3("Origin", (float *)&bufferDesc.Origin, 0, 200);
	ImGui::SliderFloat3("Velocity_Rain", (float *)&bufferDesc.Velocity, -200, 200);
	ImGui::ColorEdit3("Color_Rain", (float *)&bufferDesc.Diffuse);
	buffer->Apply();
}

void RainInst::Render()
{


	Renderer::Render();

	stride = sizeof(D3DXMATRIX);
	offset = 0;
	D3D::GetDC()->IASetVertexBuffers(1, 1, &instanceBuffer, &stride, &offset);

	shader->DrawIndexedInstanced(0, pass, 6, instCount);





}

void RainInst::CreateData()
{
	vertices = new VertexTextureNormal[4];

	vertices[0].Position = D3DXVECTOR3(-0.5f, -0.5f, 0.0f);
	vertices[1].Position = D3DXVECTOR3(-0.5f, +0.5f, 0.0f);
	vertices[2].Position = D3DXVECTOR3(+0.5f, -0.5f, 0.0f);
	vertices[3].Position = D3DXVECTOR3(+0.5f, +0.5f, 0.0f);

	vertices[0].Uv = D3DXVECTOR2(0, 1);
	vertices[1].Uv = D3DXVECTOR2(0, 0);
	vertices[2].Uv = D3DXVECTOR2(1, 1);
	vertices[3].Uv = D3DXVECTOR2(1, 0);



	vertexCount = 4;
	//
	indexCount = 6;

	indices = new UINT[indexCount]
	{
	   0, 1, 2, 2, 1, 3
	};


	for (int i = 0; i < instCount; i++)
	{
		D3DXVECTOR2 scale;
		scale.x = Math::Random(0.1f, 0.4f);
		scale.y = Math::Random(4.0f, 8.0f);

		D3DXVECTOR3 position;
		position.x = Math::Random(-initDesc.Size.x, initDesc.Size.x);
		position.y = Math::Random(-initDesc.Size.y, initDesc.Size.y);
		position.z = Math::Random(-initDesc.Size.z, initDesc.Size.z);


		D3DXMATRIX S, T;
		D3DXMatrixScaling(&S, scale.x, scale.y, 1);
		D3DXMatrixTranslation(&T, position.x, position.y, position.z);

		world[i] = S * T;

	}
	

	// CreateVertexBuffer
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(D3DXMATRIX) * MAX_COUNT;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = world;

		HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &instanceBuffer);
		assert(SUCCEEDED(hr));
	}

	//

}

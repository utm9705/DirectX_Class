#include "Framework.h"
#include "Snow.h"

Snow::Snow(InitDesc & desc)
	: Renderer(), initDesc(desc), texture(NULL)
{
	shader = new Shader(L"Snow.fx");
	SetShader(shader);

	texture = new Texture(L"Snow.png");
	shader->AsShaderResource("Base")->SetResource(texture->SRV());


	buffer = new CBuffer(&bufferDesc, sizeof(BufferDesc));
	bufferDesc.Size = desc.Size;

	sBuffer = shader->AsConstantBuffer("CB_Snow");
	sBuffer->SetConstantBuffer(buffer->Buffer());


	CreateData();

	CreateVertexBuffer(vertices, desc.Count * 4, sizeof(VertexSnow), 0);
	CreateIndexBuffer(indices, desc.Count * 6);
}

Snow::~Snow()
{
	SAFE_DELETE(shader);
	SAFE_DELETE(buffer);

	SAFE_DELETE_ARRAY(vertices);
	SAFE_DELETE_ARRAY(indices);

	SAFE_DELETE(texture);
}

void Snow::Update()
{
	Renderer::Update();

	D3DXVECTOR3 position;
	Context::Get()->GetCamera()->Position(&position);

	bufferDesc.Origin = position;

	//ImGui::SliderFloat3("Origin", (float *)&bufferDesc.Origin, 0, 200);
	ImGui::SliderFloat3("Velocity", (float *)&bufferDesc.Velocity, -200, 200);
	ImGui::SliderFloat("Turbulence", &bufferDesc.Turbulence, -20, 20);
	ImGui::ColorEdit3("Color", (float *)&bufferDesc.Diffuse);
	buffer->Apply();
}

void Snow::Render()
{
	Renderer::Render();

	shader->DrawIndexed(0, pass, initDesc.Count * 6);
}

void Snow::CreateData()
{
	vertices = new VertexSnow[initDesc.Count * 4];
	indices = new UINT[initDesc.Count * 6];

	for (UINT i = 0; i < initDesc.Count * 4; i += 4)
	{
		D3DXVECTOR3 position;
		position.x = Math::Random(-initDesc.Size.x, initDesc.Size.x);
		position.y = Math::Random(-initDesc.Size.y, initDesc.Size.y);
		position.z = Math::Random(-initDesc.Size.z, initDesc.Size.z);

		vertices[i + 0].Position = position;
		vertices[i + 1].Position = position;
		vertices[i + 2].Position = position;
		vertices[i + 3].Position = position;


		D3DXVECTOR2 random;
		random.x = Math::Random(0.0f, 1.0f);
		random.y = Math::Random(0.0f, 1.0f);

		vertices[i + 0].Random = random;
		vertices[i + 1].Random = random;
		vertices[i + 2].Random = random;
		vertices[i + 3].Random = random;


		float scale = Math::Random(0.3f, 0.6f);
		vertices[i + 0].Scale = scale;
		vertices[i + 1].Scale = scale;
		vertices[i + 2].Scale = scale;
		vertices[i + 3].Scale = scale;


		vertices[i + 0].Uv = D3DXVECTOR2(0, 1);
		vertices[i + 1].Uv = D3DXVECTOR2(0, 0);
		vertices[i + 2].Uv = D3DXVECTOR2(1, 1);
		vertices[i + 3].Uv = D3DXVECTOR2(1, 0);
	}

	for (UINT i = 0; i < initDesc.Count; i++)
	{
		indices[i * 6 + 0] = i * 4 + 0;
		indices[i * 6 + 1] = i * 4 + 1;
		indices[i * 6 + 2] = i * 4 + 2;
		indices[i * 6 + 3] = i * 4 + 2;
		indices[i * 6 + 4] = i * 4 + 1;
		indices[i * 6 + 5] = i * 4 + 3;
	}
}

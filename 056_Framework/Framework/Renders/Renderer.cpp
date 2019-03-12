#include "Framework.h"
#include "Renderer.h"

Renderer::Renderer()
	: pass(0)
	, position(0, 0, 0), scale(1, 1, 1), rotation(0, 0, 0)
	, direction(0, 0, 1), up(0, 1, 0), right(1, 0, 0)
	, vertexBuffer(NULL), indexBuffer(NULL)
{
	Initialize();
}

Renderer::Renderer(Shader * shader)
	: shader(shader), pass(0)
	, position(0, 0, 0), scale(1, 1, 1), rotation(0, 0, 0)
	, direction(0, 0, 1), up(0, 1, 0), right(1, 0, 0)
	, vertexBuffer(NULL), indexBuffer(NULL)
{
	Initialize();

	SetShader(shader);
}

void Renderer::Initialize()
{
	D3DXMatrixIdentity(&frameDesc.World);
	D3DXMatrixIdentity(&frameDesc.View);
	D3DXMatrixIdentity(&frameDesc.ViewInverse);
	D3DXMatrixIdentity(&frameDesc.Projection);

	frameBuffer = new CBuffer(&frameDesc, sizeof(FrameDesc));
}

Renderer::~Renderer()
{
	SAFE_DELETE(frameBuffer);
	
	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);
}

void Renderer::SetShader(Shader * shader)
{
	this->shader = shader;

	sFrameDesc = shader->AsConstantBuffer("CB_PerFrame");
}

void Renderer::Position(float x, float y, float z)
{
	Position(D3DXVECTOR3(x, y, z));
}

void Renderer::Position(D3DXVECTOR3 & vec)
{
	position = vec;

	UpdateWorld();
}

void Renderer::Position(D3DXVECTOR3 * vec)
{
	*vec = position;
}

void Renderer::Scale(float x, float y, float z)
{
	Scale(D3DXVECTOR3(x, y, z));
}

void Renderer::Scale(D3DXVECTOR3 & vec)
{
	scale = vec;

	UpdateWorld();
}

void Renderer::Scale(D3DXVECTOR3 * vec)
{
	*vec = scale;
}

void Renderer::Rotation(float x, float y, float z)
{
	Rotation(D3DXVECTOR3(x, y, z));
}

void Renderer::Rotation(D3DXVECTOR3 & vec)
{
	rotation = vec;

	UpdateWorld();
}

void Renderer::Rotation(D3DXVECTOR3 * vec)
{
	*vec = rotation;
}

void Renderer::RotationDegree(float x, float y, float z)
{
	RotationDegree(D3DXVECTOR3(x, y, z));
}

void Renderer::RotationDegree(D3DXVECTOR3 & vec)
{
	D3DXVECTOR3 temp;

	temp.x = Math::ToRadian(vec.x);
	temp.y = Math::ToRadian(vec.y);
	temp.z = Math::ToRadian(vec.z);

	Rotation(temp);
}

void Renderer::RotationDegree(D3DXVECTOR3 * vec)
{
	D3DXVECTOR3 temp;

	temp.x = Math::ToDegree(rotation.x);
	temp.y = Math::ToDegree(rotation.y);
	temp.z = Math::ToDegree(rotation.z);

	*vec = temp;
}

void Renderer::Direction(D3DXVECTOR3 * vec)
{
	*vec = direction;
}

void Renderer::Up(D3DXVECTOR3 * vec)
{
	*vec = up;
}

void Renderer::Right(D3DXVECTOR3 * vec)
{
	*vec = right;
}

void Renderer::Update()
{
	frameDesc.View = Context::Get()->View();
	D3DXMatrixInverse(&frameDesc.ViewInverse, NULL, &frameDesc.View);

	frameDesc.Projection = Context::Get()->Projection();
	frameDesc.Vp = frameDesc.View * frameDesc.Projection;

	frameDesc.LightColor = Context::Get()->LightColor();
	frameDesc.LightPosition = Context::Get()->LightPosition();
	frameDesc.LightDirection = Context::Get()->LightDirection();
	frameDesc.Time = Time::Get()->Running();
	frameBuffer->Apply();
}

void Renderer::Render()
{
	sFrameDesc->SetConstantBuffer(frameBuffer->Buffer());

	if (vertexBuffer != NULL)
	{
		D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

		if (indexBuffer != NULL)
			D3D::GetDC()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	}

	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Renderer::UpdateWorld()
{
	D3DXMATRIX S, R, T;
	D3DXMatrixScaling(&S, scale.x, scale.y, scale.z);
	D3DXMatrixRotationYawPitchRoll(&R, rotation.y, rotation.x, rotation.z);
	D3DXMatrixTranslation(&T, position.x, position.y, position.z);

	D3DXMATRIX world = S * R * T;
	frameDesc.World = world;

	direction = D3DXVECTOR3(world._31, world._32, world._33);
	up = D3DXVECTOR3(world._21, world._22, world._23);
	right = D3DXVECTOR3(world._11, world._12, world._13);
}

void Renderer::CreateVertexBuffer(void * pData, UINT count, UINT stride, UINT offset)
{
	D3D11_BUFFER_DESC desc = { 0 };
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = stride * count;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA data = { 0 };
	data.pSysMem = pData;

	HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer);
	assert(SUCCEEDED(hr));


	this->stride = stride;
	this->offset = offset;
}

void Renderer::CreateIndexBuffer(UINT * pData, UINT count)
{
	D3D11_BUFFER_DESC desc = { 0 };
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.ByteWidth = sizeof(UINT) * count;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA data = { 0 };
	data.pSysMem = pData;

	HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &indexBuffer);
	assert(SUCCEEDED(hr));
}

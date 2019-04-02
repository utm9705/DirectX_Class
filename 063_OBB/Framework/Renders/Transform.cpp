#include "Framework.h"
#include "Transform.h"

Transform::Transform()
	: shader(NULL), buffer(NULL)
	, position(0, 0, 0), scale(1, 1, 1), rotation(0, 0, 0)
{
	UpdateWorld();
}

Transform::Transform(Shader * shader)
	: position(0, 0, 0), scale(1, 1, 1), rotation(0, 0, 0)
{
	SetShader(shader);

	UpdateWorld();
}

Transform::~Transform()
{
	SAFE_DELETE(buffer);
}

void Transform::Set(const Transform * transform)
{
	position = transform->position;
	scale = transform->scale;
	rotation = transform->rotation;

	UpdateWorld();
}

void Transform::SetShader(Shader* shader)
{
	this->shader = shader;

	buffer = new ConstantBuffer(&bufferDesc, sizeof(BufferDesc));
	sBuffer = shader->AsConstantBuffer("CB_World");
}

void Transform::Position(float x, float y, float z)
{
	Position(D3DXVECTOR3(x, y, z));
}

void Transform::Position(D3DXVECTOR3 & vec)
{
	position = vec;

	UpdateWorld();
}

void Transform::Position(D3DXVECTOR3 * vec)
{
	*vec = position;
}

void Transform::Scale(float x, float y, float z)
{
	Scale(D3DXVECTOR3(x, y, z));
}

void Transform::Scale(D3DXVECTOR3 & vec)
{
	scale = vec;

	UpdateWorld();
}

void Transform::Scale(D3DXVECTOR3 * vec)
{
	*vec = scale;
}

void Transform::Rotation(float x, float y, float z)
{
	Rotation(D3DXVECTOR3(x, y, z));
}

void Transform::Rotation(D3DXVECTOR3 & vec)
{
	rotation = vec;

	UpdateWorld();
}

void Transform::Rotation(D3DXVECTOR3 * vec)
{
	*vec = rotation;
}

void Transform::RotationDegree(float x, float y, float z)
{
	RotationDegree(D3DXVECTOR3(x, y, z));
}

void Transform::RotationDegree(D3DXVECTOR3 & vec)
{
	D3DXVECTOR3 temp;

	temp.x = Math::ToRadian(vec.x);
	temp.y = Math::ToRadian(vec.y);
	temp.z = Math::ToRadian(vec.z);

	Rotation(temp);
}

void Transform::RotationDegree(D3DXVECTOR3 * vec)
{
	D3DXVECTOR3 temp;

	temp.x = Math::ToDegree(rotation.x);
	temp.y = Math::ToDegree(rotation.y);
	temp.z = Math::ToDegree(rotation.z);

	*vec = temp;
}

D3DXVECTOR3 Transform::Direction()
{
	return D3DXVECTOR3(bufferDesc.World._31, bufferDesc.World._32, bufferDesc.World._33);
}

D3DXVECTOR3 Transform::Up()
{
	return D3DXVECTOR3(bufferDesc.World._21, bufferDesc.World._22, bufferDesc.World._23);
}

D3DXVECTOR3 Transform::Right()
{
	return D3DXVECTOR3(bufferDesc.World._11, bufferDesc.World._12, bufferDesc.World._13);
}

void Transform::UpdateWorld()
{
	D3DXMATRIX S, R, T;
	D3DXMatrixScaling(&S, scale.x, scale.y, scale.z);
	D3DXMatrixRotationYawPitchRoll(&R, rotation.y, rotation.x, rotation.z);
	D3DXMatrixTranslation(&T, position.x, position.y, position.z);

	D3DXMATRIX world = S * R * T;
	bufferDesc.World = world;

	for (function<void()>& func : onChaned)
		func();
}

void Transform::Update()
{

}

void Transform::Render()
{
	if (shader == NULL) return;

	buffer->Apply();
	sBuffer->SetConstantBuffer(buffer->Buffer());
}

void Transform::PushOnChanged(function<void()> f)
{
	onChaned.push_back(f);
}

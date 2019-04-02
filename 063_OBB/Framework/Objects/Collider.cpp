#include "Framework.h"
#include "Collider.h"

Collider::Collider(wstring name, Transform * transform)
	: name(name)
	, transform(transform), color(0, 1, 0, 1)
{
	lines[0] = D3DXVECTOR3(-0.5f, -0.5f, -0.5f);
	lines[1] = D3DXVECTOR3(-0.5f, +0.5f, -0.5f);
	lines[2] = D3DXVECTOR3(+0.5f, -0.5f, -0.5f);
	lines[3] = D3DXVECTOR3(+0.5f, +0.5f, -0.5f);
	lines[4] = D3DXVECTOR3(-0.5f, -0.5f, +0.5f);
	lines[5] = D3DXVECTOR3(-0.5f, +0.5f, +0.5f);
	lines[6] = D3DXVECTOR3(+0.5f, -0.5f, +0.5f);
	lines[7] = D3DXVECTOR3(+0.5f, +0.5f, +0.5f);


	SetObb();
}

Collider::~Collider()
{
}

bool Collider::IsIntersect(Collider * other)
{
	return CalcCollision(this->obb, other->obb);
}

void Collider::Update()
{
	SetObb();
}

void Collider::Render()
{
	D3DXVECTOR3 dest[8];
	D3DXMATRIX world = transform->World();

	for (UINT i = 0; i < 8; i++)
		D3DXVec3TransformCoord(&dest[i], &lines[i], &world);


	//Front
	DebugLine::Get()->RenderLine(dest[0], dest[1], color);
	DebugLine::Get()->RenderLine(dest[1], dest[3], color);
	DebugLine::Get()->RenderLine(dest[3], dest[2], color);
	DebugLine::Get()->RenderLine(dest[2], dest[0], color);

	//Backward
	DebugLine::Get()->RenderLine(dest[4], dest[5], color);
	DebugLine::Get()->RenderLine(dest[5], dest[7], color);
	DebugLine::Get()->RenderLine(dest[7], dest[6], color);
	DebugLine::Get()->RenderLine(dest[6], dest[4], color);

	//Side
	DebugLine::Get()->RenderLine(dest[0], dest[4], color);
	DebugLine::Get()->RenderLine(dest[1], dest[5], color);
	DebugLine::Get()->RenderLine(dest[2], dest[6], color);
	DebugLine::Get()->RenderLine(dest[3], dest[7], color);
}

void Collider::Color(float r, float g, float b, float a)
{
	Color(D3DXCOLOR(r, g, b, a));
}

void Collider::Color(D3DXCOLOR & color)
{
	this->color = color;
}

bool Collider::CalcSperatingPlane(D3DXVECTOR3 & position, D3DXVECTOR3 & plane, Obb & box1, Obb & box2)
{
	float val = fabsf(D3DXVec3Dot(&position, &plane));

	float val2 = 0.0f;
	val2 += fabsf(D3DXVec3Dot(&(box1.AxisX * box1.HalfSize.x), &plane));
	val2 += fabsf(D3DXVec3Dot(&(box1.AxisY * box1.HalfSize.y), &plane));
	val2 += fabsf(D3DXVec3Dot(&(box1.AxisZ * box1.HalfSize.z), &plane));
	val2 += fabsf(D3DXVec3Dot(&(box2.AxisX * box2.HalfSize.x), &plane));
	val2 += fabsf(D3DXVec3Dot(&(box2.AxisY * box2.HalfSize.y), &plane));
	val2 += fabsf(D3DXVec3Dot(&(box2.AxisZ * box2.HalfSize.z), &plane));

	return val > val2;
}

bool Collider::CalcCollision(Obb & box1, Obb & box2)
{
	D3DXVECTOR3 position = box2.Position - box1.Position;

	if (CalcSperatingPlane(position, box1.AxisX, box1, box2) == true) return false;
	if (CalcSperatingPlane(position, box1.AxisY, box1, box2) == true) return false;
	if (CalcSperatingPlane(position, box1.AxisZ, box1, box2) == true) return false;

	if (CalcSperatingPlane(position, box2.AxisX, box1, box2) == true) return false;
	if (CalcSperatingPlane(position, box2.AxisY, box1, box2) == true) return false;
	if (CalcSperatingPlane(position, box2.AxisZ, box1, box2) == true) return false;

	if (CalcSperatingPlane(position, Cross(box1.AxisX, box2.AxisX), box1, box2) == true) return false;
	if (CalcSperatingPlane(position, Cross(box1.AxisX, box2.AxisY), box1, box2) == true) return false;
	if (CalcSperatingPlane(position, Cross(box1.AxisX, box2.AxisZ), box1, box2) == true) return false;

	if (CalcSperatingPlane(position, Cross(box1.AxisY, box2.AxisX), box1, box2) == true) return false;
	if (CalcSperatingPlane(position, Cross(box1.AxisY, box2.AxisY), box1, box2) == true) return false;
	if (CalcSperatingPlane(position, Cross(box1.AxisY, box2.AxisZ), box1, box2) == true) return false;

	if (CalcSperatingPlane(position, Cross(box1.AxisZ, box2.AxisX), box1, box2) == true) return false;
	if (CalcSperatingPlane(position, Cross(box1.AxisZ, box2.AxisY), box1, box2) == true) return false;
	if (CalcSperatingPlane(position, Cross(box1.AxisZ, box2.AxisZ), box1, box2) == true) return false;


	return true;
}

void Collider::SetObb()
{
	transform->Position(&obb.Position);

	obb.AxisX = transform->Right();
	obb.AxisY = transform->Up();
	obb.AxisZ = transform->Direction();

	D3DXVECTOR3 scale;
	transform->Scale(&scale);
	obb.HalfSize.x = scale.x * 0.5f;
	obb.HalfSize.y = scale.y * 0.5f;
	obb.HalfSize.z = scale.z * 0.5f;
}

D3DXVECTOR3 Collider::Cross(D3DXVECTOR3 vec1, D3DXVECTOR3 vec2)
{
	float x = vec1.y * vec2.z - vec1.z * vec2.y;
	float y = vec1.z * vec2.x - vec1.x * vec2.z;
	float z = vec1.x * vec2.y - vec1.y * vec2.x;

	return D3DXVECTOR3(x, y, z);
}
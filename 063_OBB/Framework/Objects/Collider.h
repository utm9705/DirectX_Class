#pragma once
#pragma once

class Collider
{
private:
	struct Obb;

public:
	Collider(wstring name, Transform* transform);
	~Collider();

	const wstring& Name() { return name; }

	bool IsIntersect(Collider* other);
	void Update();
	void Render();

	void Color(float r, float g, float b, float a);
	void Color(D3DXCOLOR& color);

private:
	bool CalcSperatingPlane(D3DXVECTOR3& position, D3DXVECTOR3& plane, Obb& box1, Obb& box2);
	bool CalcCollision(Obb& box1, Obb& box2);

	void SetObb();
	D3DXVECTOR3 Cross(D3DXVECTOR3 vec1, D3DXVECTOR3 vec2);

private:
	struct Obb
	{
		D3DXVECTOR3 Position;
		D3DXVECTOR3 AxisX;
		D3DXVECTOR3 AxisY;
		D3DXVECTOR3 AxisZ;
		D3DXVECTOR3 HalfSize;
	};

private:
	wstring name;
	Transform* transform;

	Obb obb;
	D3DXCOLOR color;

	D3DXVECTOR3 lines[8];
};
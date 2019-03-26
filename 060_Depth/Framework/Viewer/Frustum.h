#pragma once

class Frustum
{
public:
	Frustum(Camera* camera = NULL, Perspective* perspective = NULL);
	~Frustum();

	void Update();

	void GetPlanes(OUT D3DXPLANE* plane);

	bool ContainPoint(D3DXVECTOR3& position);
	bool ContainRect(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize);
	bool ContainRect(D3DXVECTOR3 center, D3DXVECTOR3 size);
	bool ContainCube(D3DXVECTOR3& center, float radius);

private:
	D3DXPLANE planes[6];

	Camera* camera;
	Perspective* perspective;
};
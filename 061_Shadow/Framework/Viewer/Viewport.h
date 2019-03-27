#pragma once

class Viewport
{
public:
	Viewport(UINT width, UINT height, float x = 0, float y = 0, float minDepth = 0, float maxDepth = 1);
	~Viewport();

	void RSSetViewport();
	void Set(UINT width, UINT height, float x = 0, float y = 0, float minDepth = 0, float maxDepth = 1);

	float GetWidth() { return width; }
	float GetHeight() { return height; }

	void GetRay
	(
		OUT D3DXVECTOR3* position, OUT D3DXVECTOR3* direction,
		D3DXVECTOR3& cameraPosition, D3DXMATRIX& world, D3DXMATRIX& view, D3DXMATRIX& projection
	);

	void Project(D3DXVECTOR2* out, D3DXVECTOR3& source, D3DXMATRIX& world, D3DXMATRIX& view, D3DXMATRIX& projection);
	void Project(D3DXVECTOR3* out, D3DXVECTOR3& source, D3DXMATRIX& world, D3DXMATRIX& view, D3DXMATRIX& projection);
	void Unproject(D3DXVECTOR3* out, D3DXVECTOR3& source, D3DXMATRIX& world, D3DXMATRIX& view, D3DXMATRIX& projection);

private:
	float x, y;
	float width, height;
	float minDepth, maxDepth;

	D3D11_VIEWPORT viewport;
};
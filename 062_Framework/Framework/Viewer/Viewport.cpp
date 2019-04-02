#include "Framework.h"
#include "Viewport.h"

Viewport::Viewport(UINT width, UINT height, float x, float y, float minDepth, float maxDepth)
{
	Set(width, height, x, y, minDepth, maxDepth);
}

Viewport::~Viewport()
{

}

void Viewport::Set(UINT width, UINT height, float x, float y, float minDepth, float maxDepth)
{
	viewport.TopLeftX = this->x = x;
	viewport.TopLeftY = this->y = y;
	viewport.Width = this->width = (float)width;
	viewport.Height = this->height = (float)height;
	viewport.MinDepth = this->minDepth = minDepth;
	viewport.MaxDepth = this->maxDepth = maxDepth;

	RSSetViewport();
}

void Viewport::GetRay(OUT D3DXVECTOR3 * position, OUT D3DXVECTOR3 * direction, D3DXVECTOR3 & cameraPosition, D3DXMATRIX & world, D3DXMATRIX & view, D3DXMATRIX & projection)
{
	D3DXVECTOR2 screen;
	screen.x = width;
	screen.y = height;

	D3DXVECTOR3 mouse = Mouse::Get()->GetPosition();


	D3DXVECTOR2 point;
	//Inv Viewport
	{
		point.x = ((2.0f * mouse.x) / screen.x) - 1.0f;
		point.y = (((2.0f * mouse.y) / screen.y) - 1.0f) * -1.0f;
	}

	//Inv Projection
	{
		point.x = point.x / projection._11;
		point.y = point.y / projection._22;
	}

	//Inv View
	{
		D3DXMATRIX invView;
		D3DXMatrixInverse(&invView, NULL, &view);

		D3DXVec3TransformNormal(direction, &D3DXVECTOR3(point.x, point.y, 1), &invView);
		D3DXVec3Normalize(direction, direction);
	}

	//Inv World
	{
		D3DXMATRIX invWorld;
		D3DXMatrixInverse(&invWorld, NULL, &world);

		D3DXVec3TransformCoord(position, &cameraPosition, &invWorld);
		D3DXVec3TransformNormal(direction, direction, &invWorld);
		D3DXVec3Normalize(direction, direction);
	}
}

void Viewport::Project(D3DXVECTOR2 * out, D3DXVECTOR3 & source, D3DXMATRIX & world, D3DXMATRIX & view, D3DXMATRIX & projection)
{
	D3DXVECTOR3 temp;
	Project(&temp, source, world, view, projection);

	*out = D3DXVECTOR2(temp.x, temp.y);
}

void Viewport::Project(D3DXVECTOR3 * out, D3DXVECTOR3 & source, D3DXMATRIX & world, D3DXMATRIX & view, D3DXMATRIX & projection)
{
	D3DXMATRIX matrix = world * view * projection;

	D3DXVECTOR3 temp = source;
	D3DXVec3TransformCoord(out, &temp, &matrix);

	out->x = ((out->x + 1.0f) * 0.5f) * width + x;
	out->y = ((-out->y + 1.0f) * 0.5f) * height + y;
	out->z = (out->z * (maxDepth - minDepth)) + minDepth;
}

void Viewport::Unproject(D3DXVECTOR3 * out, D3DXVECTOR3 & source, D3DXMATRIX & world, D3DXMATRIX & view, D3DXMATRIX & projection)
{
	D3DXMATRIX matrix = world * view * projection;
	D3DXMatrixInverse(&matrix, NULL, &matrix);

	D3DXVECTOR3 temp = source;
	out->x = ((temp.x - x) / width) * 2.0f - 1.0f;
	out->y = (((temp.y - y) / height) * 2.0f - 1.0f) * -1.0f;
	out->z = (temp.z - minDepth) / (maxDepth - minDepth);

	D3DXVec3TransformCoord(out, out, &matrix);
}

void Viewport::RSSetViewport()
{
	D3D::GetDC()->RSSetViewports(1, &viewport);
}
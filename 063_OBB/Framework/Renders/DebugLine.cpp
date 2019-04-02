#include "Framework.h"
#include "DebugLine.h"

DebugLine* DebugLine::instance = NULL;

void DebugLine::Create()
{
	assert(instance == NULL);

	instance = new DebugLine();
}

void DebugLine::Delete()
{
	SAFE_DELETE(instance);
}

DebugLine * DebugLine::Get()
{
	assert(instance != NULL);

	return instance;
}

void DebugLine::RenderLine(D3DXVECTOR3 & start, D3DXVECTOR3 & end)
{
	RenderLine(start, end, D3DXCOLOR(0, 1, 0, 1));
}

void DebugLine::RenderLine(D3DXVECTOR3 & start, D3DXVECTOR3 & end, float r, float g, float b)
{
	RenderLine(start, end, D3DXCOLOR(r, g, b, 1));
}

void DebugLine::RenderLine(float x, float y, float z, float x2, float y2, float z2)
{
	RenderLine(D3DXVECTOR3(x, y, z), D3DXVECTOR3(x2, y2, z2), D3DXCOLOR(0, 1, 0, 1));
}

void DebugLine::RenderLine(float x, float y, float z, float x2, float y2, float z2, float r, float g, float b)
{
	RenderLine(D3DXVECTOR3(x, y, z), D3DXVECTOR3(x2, y2, z2), D3DXCOLOR(r, g, b, 1));
}

void DebugLine::RenderLine(float x, float y, float z, float x2, float y2, float z2, D3DXCOLOR & color)
{
	RenderLine(D3DXVECTOR3(x, y, z), D3DXVECTOR3(x2, y2, z2), color);
}

void DebugLine::RenderLine(D3DXVECTOR3 & start, D3DXVECTOR3 & end, D3DXCOLOR & color)
{
	vertices[drawCount].Color = color;
	vertices[drawCount++].Position = start;

	vertices[drawCount].Color = color;
	vertices[drawCount++].Position = end;
}

void DebugLine::Render()
{
	perFrame->Update();
	transform->Update();

	perFrame->Render();
	transform->Render();


	D3D11_MAPPED_SUBRESOURCE subResource;
	D3D::GetDC()->Map(vertexBuffer->Buffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		memcpy(subResource.pData, vertices, sizeof(Vertex) * MAX_LINE_VERTEX);
	}
	D3D::GetDC()->Unmap(vertexBuffer->Buffer(), 0);


	vertexBuffer->Render();
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	shader->Draw(0, 0, drawCount * 2);

	drawCount = 0;
	ZeroMemory(vertices, sizeof(Vertex) * MAX_LINE_VERTEX);
}

DebugLine::DebugLine()
	: drawCount(0)
{
	shader = new Shader(L"063_DebugLine.fx");

	vertices = new VertexColor[MAX_LINE_VERTEX];
	ZeroMemory(vertices, sizeof(VertexColor) * MAX_LINE_VERTEX);

	vertexBuffer = new VertexBuffer(vertices, MAX_LINE_VERTEX, sizeof(VertexColor));

	perFrame = new PerFrame(shader);
	transform = new Transform(shader);
}

DebugLine::~DebugLine()
{
	SAFE_DELETE_ARRAY(vertices);
	SAFE_DELETE(vertexBuffer);

	SAFE_DELETE(transform);
	SAFE_DELETE(perFrame);
	SAFE_DELETE(shader);
}
#pragma once
#define MAX_LINE_VERTEX 10000

class DebugLine
{
public:
	friend class Window;

public:
	static void Create();
	static void Delete();

	static DebugLine* Get();

public:
	void RenderLine(D3DXVECTOR3& start, D3DXVECTOR3& end);
	void RenderLine(D3DXVECTOR3& start, D3DXVECTOR3& end, float r, float g, float b);

	void RenderLine(float x, float y, float z, float x2, float y2, float z2);
	void RenderLine(float x, float y, float z, float x2, float y2, float z2, D3DXCOLOR& color);
	void RenderLine(float x, float y, float z, float x2, float y2, float z2, float r, float g, float b);

	void RenderLine(D3DXVECTOR3& start, D3DXVECTOR3& end, D3DXCOLOR& color);


private:
	void Render();

private:
	DebugLine();
	~DebugLine();

private:
	static DebugLine* instance;

private:
	Shader* shader;
	PerFrame* perFrame;
	Transform* transform;

	VertexBuffer* vertexBuffer;

	VertexColor* vertices;
	UINT drawCount = 0;
};
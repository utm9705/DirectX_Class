#pragma once

class Mesh : public Renderer
{
public:
	typedef VertexTextureNormalTangent MeshVertex;

public:
	Mesh(Shader* shader);
	virtual ~Mesh();

	void Render();

	void Color(D3DXCOLOR& val);
	void Color(float r, float g, float b);

protected:
	virtual void Create() = 0;

protected:
	MeshVertex* vertices;
	UINT* indices;

	UINT vertexCount;
	UINT indexCount;

	D3DXCOLOR color;
};
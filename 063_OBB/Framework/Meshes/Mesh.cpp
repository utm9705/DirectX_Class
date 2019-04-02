#include "Framework.h"
#include "Mesh.h"

Mesh::Mesh(Shader * shader)
	: Renderer(shader)
{
	
}

Mesh::~Mesh()
{
	SAFE_DELETE_ARRAY(vertices);
	SAFE_DELETE_ARRAY(indices);
}

void Mesh::Render()
{
	if (vertexBuffer == NULL && indexBuffer == NULL)
	{
		Create();

		vertexBuffer = new VertexBuffer(vertices, vertexCount, sizeof(MeshVertex), 0);
		indexBuffer = new IndexBuffer(indices, indexCount);
	}

	Renderer::Render();

	shader->DrawIndexed(0, Pass(), indexCount);
}
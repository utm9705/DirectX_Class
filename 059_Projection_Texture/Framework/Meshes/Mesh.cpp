#include "Framework.h"
#include "Mesh.h"

Mesh::Mesh(Shader * shader)
	: Renderer(shader)
	, diffuseMap(NULL), specularMap(NULL), normalMap(NULL)
{
	sDiffuseMap = shader->AsShaderResource("DiffuseMap");
	sSpecularMap = shader->AsShaderResource("SpecularMap");
	sNormalMap = shader->AsShaderResource("NormalMap");
}

Mesh::~Mesh()
{
	SAFE_DELETE(diffuseMap);
	SAFE_DELETE(specularMap);
	SAFE_DELETE(normalMap);

	SAFE_DELETE_ARRAY(vertices);
	SAFE_DELETE_ARRAY(indices);
}

void Mesh::DiffuseMap(wstring file)
{
	SAFE_DELETE(diffuseMap);

	diffuseMap = new Texture(file);
}

void Mesh::SpecularMap(wstring file)
{
	SAFE_DELETE(specularMap);

	specularMap = new Texture(file);
}

void Mesh::NormalMap(wstring file)
{
	SAFE_DELETE(normalMap);

	normalMap = new Texture(file);
}

void Mesh::Render()
{
	if (vertexBuffer == NULL && indexBuffer == NULL)
	{
		Create();

		CreateVertexBuffer(vertices, vertexCount, sizeof(MeshVertex), 0);
		CreateIndexBuffer(indices, indexCount);
	}

	Renderer::Render();

	if (diffuseMap != NULL)
		sDiffuseMap->SetResource(diffuseMap->SRV());

	if (normalMap != NULL)
		sNormalMap->SetResource(normalMap->SRV());

	if (specularMap != NULL)
		sSpecularMap->SetResource(specularMap->SRV());


	shader->DrawIndexed(0, pass, indexCount);
}
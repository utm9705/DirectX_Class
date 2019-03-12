#include "framework.h"
#include "ModelMesh.h"
#include "ModelMeshPart.h"
#include "ModelBone.h"
#include "ModelInstance.h"

ModelMesh::ModelMesh()
	: Renderer()
{
	
}

ModelMesh::~ModelMesh()
{
	SAFE_DELETE_ARRAY(vertices);
	SAFE_DELETE_ARRAY(indices);

	for (ModelMeshPart* part : meshParts)
		SAFE_DELETE(part);

	SAFE_RELEASE(instanceBuffer);
}

void ModelMesh::SetShader(Shader * shader)
{
	Renderer::SetShader(shader);

	sBoneIndex = shader->AsScalar("BoneIndex");
	sTransforms = shader->AsShaderResource("Transforms");
}

void ModelMesh::Render()
{
	Renderer::Render();

	sBoneIndex->SetInt(parentBoneIndex);
	for (ModelMeshPart* part : meshParts)
		part->Render();
}

void ModelMesh::RenderInstance(UINT count)
{
	Renderer::Render();

	stride = sizeof(D3DXMATRIX);
	D3D::GetDC()->IASetVertexBuffers(1, 1, &instanceBuffer, &stride, &offset);


	sBoneIndex->SetInt(parentBoneIndex);
	sTransforms->SetResource(transformsSRV);

	for (ModelMeshPart* part : meshParts)
		part->RenderInstance(count);
}

void ModelMesh::Copy(ModelMesh ** clone)
{
	ModelMesh* mesh = new ModelMesh();
	mesh->name = name;
	mesh->parentBoneIndex = parentBoneIndex;

	mesh->vertexCount = this->vertexCount;
	mesh->vertices = new VertexTextureNormalBlend[this->vertexCount];
	memcpy(mesh->vertices, this->vertices, sizeof(VertexTextureNormalBlend) * vertexCount);

	mesh->indexCount = this->indexCount;
	mesh->indices = new UINT[this->indexCount];
	memcpy(mesh->indices, this->indices, sizeof(UINT) * indexCount);

	for (ModelMeshPart* part : meshParts)
	{
		ModelMeshPart* temp = NULL;
		part->Copy(&temp);

		temp->parent = mesh;
		mesh->meshParts.push_back(temp);
	}
	*clone = mesh;
}

void ModelMesh::TransformsSRV(ID3D11ShaderResourceView * srv)
{
	transformsSRV = srv;

	sTransforms->SetResource(transformsSRV);
}

void ModelMesh::Binding()
{
	CreateVertexBuffer(vertices, vertexCount, sizeof(VertexTextureNormalBlend), 0);
	CreateIndexBuffer(indices, indexCount);

	CsResource::CreateRawBuffer(sizeof(D3DXMATRIX) * MAX_INST_MODEL, NULL, &instanceBuffer);
}

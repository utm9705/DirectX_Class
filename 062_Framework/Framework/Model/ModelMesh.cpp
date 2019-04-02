#include "Framework.h"
#include "ModelMesh.h"
#include "ModelMeshPart.h"

ModelBone::ModelBone()
	: parent(NULL)
{

}

ModelBone::~ModelBone()
{

}

///////////////////////////////////////////////////////////////////////////////

ModelMesh::ModelMesh()
{
	
}

ModelMesh::~ModelMesh()
{
	SAFE_DELETE(vertexBuffer);
	SAFE_DELETE(indexBuffer);

	SAFE_DELETE_ARRAY(vertices);
	SAFE_DELETE_ARRAY(indices);

	for (ModelMeshPart* part : meshParts)
		SAFE_DELETE(part);
}

void ModelMesh::Binding(Model * model)
{
	vertexBuffer = new VertexBuffer(vertices, vertexCount, sizeof(Model::ModelVertex));
	indexBuffer = new IndexBuffer(indices, indexCount);

	for (ModelMeshPart* part : meshParts)
		part->Binding(model);
}

void ModelMesh::Pass(UINT val)
{
	for (ModelMeshPart* part : meshParts)
		part->Pass(val);
}

void ModelMesh::SetShader(Shader * shader)
{
	for (ModelMeshPart* part : meshParts)
		part->SetShader(shader);
}

//void ModelMesh::Transforms(D3DXMATRIX * transforms)
//{
//	for (ModelMeshPart* part : meshParts)
//		part->Transforms(transforms);
//}

//void ModelMesh::SetTransform(Transform * transform)
//{
//	for (ModelMeshPart* part : meshParts)
//		part->SetTransform(transform);
//}

void ModelMesh::TransformsSRV(ID3D11ShaderResourceView * srv)
{
	for (ModelMeshPart* part : meshParts)
		part->TransformsSRV(srv);
}

void ModelMesh::Update()
{
	for (ModelMeshPart* part : meshParts)
		part->Update();
}

void ModelMesh::Render(UINT count)
{
	vertexBuffer->Render();
	indexBuffer->Render();

	for (ModelMeshPart* part : meshParts)
		part->Render(count);
}
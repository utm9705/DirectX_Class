#include "framework.h"
#include "ModelMesh.h"
#include "ModelMeshPart.h"
#include "ModelBone.h"

ModelMesh::ModelMesh()
{
	
}

ModelMesh::~ModelMesh()
{
	SAFE_DELETE_ARRAY(vertices);
	SAFE_DELETE_ARRAY(indices);

	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);

	for (ModelMeshPart* part : meshParts)
		SAFE_DELETE(part);
}

void ModelMesh::Render()
{
	UINT stride = sizeof(VertexTextureNormalBlend);
	UINT offset = 0;

	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	for (ModelMeshPart* part : meshParts)
		part->Render();
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

void ModelMesh::Pass(UINT pass)
{
	for (ModelMeshPart* part : meshParts)
		part->Pass(pass);
}

void ModelMesh::Binding()
{
	CsResource::CreateRawBuffer(sizeof(VertexTextureNormalBlend) * vertexCount, vertices, &vertexBuffer);


	D3D11_BUFFER_DESC desc;
	D3D11_SUBRESOURCE_DATA data;

	//Index Buffer
	{
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.ByteWidth = sizeof(UINT) * indexCount;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
		data.pSysMem = indices;

		HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &indexBuffer);
		assert(SUCCEEDED(hr));
	}

}

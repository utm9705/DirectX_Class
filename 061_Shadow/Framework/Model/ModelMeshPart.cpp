#include "framework.h"
#include "ModelMeshPart.h"
#include "ModelMesh.h"

ModelMeshPart::ModelMeshPart()
	: material(NULL)
{
}

ModelMeshPart::~ModelMeshPart()
{
}

void ModelMeshPart::Copy(ModelMeshPart ** clone)
{
	ModelMeshPart* part = new ModelMeshPart();

	part->materialName = materialName;

	part->startVertex = startVertex;
	part->vertexCount = vertexCount;

	part->startIndex = startIndex;
	part->indexCount = indexCount;

	*clone = part;
}

void ModelMeshPart::Render()
{
	material->Render();

	//parent->GetShader()->AsVector("Planes")->SetFloatVector(planes);
	parent->GetShader()->DrawIndexed(0, parent->Pass(), indexCount, startIndex, 0);
}

void ModelMeshPart::RenderInstance(UINT count)
{
	material->Render();

	//parent->GetShader()->AsVector("Planes")->SetFloatVector(planes);
	parent->GetShader()->DrawIndexedInstanced(0, parent->Pass(), indexCount, count, startIndex);
}

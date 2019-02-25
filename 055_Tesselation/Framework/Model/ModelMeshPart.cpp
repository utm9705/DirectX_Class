#include "framework.h"
#include "ModelMeshPart.h"
#include "ModelMesh.h"

void ModelMeshPart::Pass(UINT pass)
{
	this->pass = pass;
}

ModelMeshPart::ModelMeshPart()
	: pass(0), sBoneIndex(NULL)
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
	if (sBoneIndex == NULL)
		sBoneIndex = material->GetShader()->AsScalar("BoneIndex");

	sBoneIndex->SetInt(parent->ParentBoneIndex());
	//material->GetShader()->Draw(0, pass, vertexCount, startVertex);
	material->GetShader()->DrawIndexed(0, pass, indexCount, startIndex, 0);
}

void ModelMeshPart::RenderInstance(UINT count)
{
	if (sBoneIndex == NULL)
		sBoneIndex = material->GetShader()->AsScalar("BoneIndex");

	sBoneIndex->SetInt(parent->ParentBoneIndex());
	//material->GetShader()->Draw(0, pass, vertexCount, startVertex);

	material->GetShader()->DrawIndexedInstanced(0, pass, indexCount, count, startIndex, 0);
}

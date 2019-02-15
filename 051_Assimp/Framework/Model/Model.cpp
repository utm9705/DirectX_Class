#include "framework.h"
#include "Model.h"
#include "ModelBone.h"
#include "ModelMesh.h"

Model::Model()
{

}

Model::~Model()
{
	for (Material* material : materials)
		SAFE_DELETE(material);

	for (ModelBone* bone : bones)
		SAFE_DELETE(bone);

	for (ModelMesh* mesh : meshes)
		SAFE_DELETE(mesh);
}

inline Material * Model::MaterialByName(wstring name)
{
	for (Material* material : materials)
	{
		if (material->Name() == name)
			return material;
	}
}

ModelBone * Model::BoneByName(wstring name)
{
	for (ModelBone* bone : bones)
	{
		if (bone->Name() == name)
			return bone;
	}

	return NULL;
}

ModelMesh * Model::MeshByName(wstring name)
{
	for (ModelMesh* mesh : meshes)
	{
		if (mesh->Name() == name)
			return mesh;
	}

	return NULL;
}
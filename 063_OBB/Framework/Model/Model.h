#pragma once

#define MAX_MODEL_TRANFORMS 128

class ModelBone;
class ModelMesh;

class Model
{
public:
	typedef VertexTextureNormalTangent ModelVertex;

public:
	Model();
	~Model();

	UINT MaterialCount() { return materials.size(); }
	vector<Material *>& Materials() { return materials; }
	Material* MaterialByIndex(UINT index) { return materials[index]; }
	Material* MaterialByName(wstring name);

	UINT BoneCount() { return bones.size(); }
	vector<ModelBone *>& Bones() { return bones; }
	ModelBone* BoneByIndex(UINT index) { return bones[index]; }
	ModelBone* BoneByName(wstring name);

	UINT MeshCount() { return meshes.size(); }
	vector<ModelMesh *>& Meshes() { return meshes; }
	ModelMesh* MeshByIndex(UINT index) { return meshes[index]; }
	ModelMesh* MeshByName(wstring name);


	void ReadMaterial(wstring file);
	void ReadMesh(wstring file);

	wstring MaterialFile() { return materialFile; }
	wstring MeshFile() { return meshFile; }

private:
	void BindingBone();
	void BindingMesh();

private:
	wstring materialFile;
	wstring meshFile;

	vector<Material *> materials;

	ModelBone* root;
	vector<ModelBone *> bones;
	vector<ModelMesh *> meshes;
};
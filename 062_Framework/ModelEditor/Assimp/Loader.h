#pragma once

class Loader
{
public:
	Loader();
	~Loader();

	void ReadFile(wstring file);

public:
	void ExportMaterial(wstring savePath, bool bOverWrite = true);

private:
	void ReadMaterial();
	void WriteMaterial(wstring savePath, bool bOverWrite = true);
	string WriteTexture(string file, string saveFolder);

public:
	void ExportMesh(wstring savePath, bool bOverWrite = true);

private:
	void ReadBoneData(aiNode* node, int index, int parent);
	void ReadMeshData(aiNode* node, int bone);
	void WriteMeshData(wstring savePath, bool bOverWrite = true);



private:
	wstring fbxFile;

	Assimp::Importer* importer;
	const aiScene* scene;

	vector<struct AsMaterial *> materials;
	vector<struct AsBone *> bones;
	vector<struct AsMesh *> meshes;
};
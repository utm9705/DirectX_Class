#pragma once

namespace Xml
{
	class XMLDocument;
	class XMLElement;
}

class Loader
{
public:
	Loader();
	~Loader();

	void ReadFile(wstring file);
	void ExportMaterial(wstring savePath, bool bOverWrite = true);
	void ExportMesh(wstring savePath, bool bOverWrite = true);
	void ExportAnimation(UINT index, wstring savePath, bool bOverWrite = true);

private:
	void ReadMaterial();
	void WriteMaterial(wstring savePath, bool bOverWrite);
	string WriteTexture(string file, string saveFolder);

	void ReadBoneData(aiNode* node, int index, int parent);
	void ReadSkinData();
	void ReadMeshData(aiNode* node, int parentBone);
	UINT FindBoneIndex(string name);
	void WriteMeshData(wstring savePath, bool bOverWrite = true);

	struct AsClip* ReadClipData(UINT index);
	void WriteClipData(struct AsClip* clip, wstring savePath, bool bOverWrite);
	void ReadKeyframeData(AsClip* clip, aiNode* node, vector<struct AsAniNode>& aniNodeInfos);
	struct AsAniNode* FindAniNode(vector<struct AsAniNode>& aniNodeInfos, aiString name);

private:
	Assimp::Importer* importer;
	const aiScene* scene;

	wstring fbxFile;

	vector<struct AsMaterial *> materials;
	vector<struct AsBone *> bones;
	vector<struct AsMesh *> meshes;
};
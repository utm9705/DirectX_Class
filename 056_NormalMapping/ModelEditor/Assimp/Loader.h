#pragma once

namespace Xml
{
	class XMLDocument;
	class XMLElement;
}

class Loader
{
public:
	Loader(wstring file, wstring saveFolder, wstring saveName);
	~Loader();

	void ExportMaterial(wstring saveFolder = L"", wstring fileName = L"");
	void ExportMesh(wstring saveFolder = L"", wstring fileName = L"");
	void ExportAnimation(UINT index, wstring saveFolder = L"", wstring fileName = L"");

private:
	void ReadMaterial();
	void WriteMaterial(wstring saveFolder, wstring fileName);
	void WriteXmlColor(Xml::XMLDocument* document, Xml::XMLElement* element, D3DXCOLOR& color);
	string WriteTexture(string file);

	void ReadBoneData(aiNode* node, int index, int parent);
	void ReadSkinData();
	void ReadMeshData(aiNode* node, int parentBone);
	UINT FindBoneIndex(string name);
	void WriteMeshData(wstring saveFolder, wstring fileName);

	struct AsClip* ReadClipData(UINT index);
	void WriteClipData(struct AsClip* clip, wstring saveFolder, wstring fileName);
	void ReadKeyframeData(AsClip* clip, aiNode* node, vector<struct AsAniNode>& aniNodeInfos);
	struct AsAniNode* FindAniNode(vector<struct AsAniNode>& aniNodeInfos, aiString name);

private:
	Assimp::Importer* importer;
	const aiScene* scene;

	wstring fbxFile;
	wstring saveFolder;
	wstring saveName;

	vector<struct AsMaterial *> materials;
	vector<struct AsBone *> bones;
	vector<struct AsMesh *> meshes;
};
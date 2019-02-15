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

private:
	void ReadMaterial();
	void WriteMaterial(wstring saveFolder, wstring fileName);
	void WriteXmlColor(Xml::XMLDocument* document, Xml::XMLElement* element, D3DXCOLOR& color);
	string WriteTexture(string file);


	void ReadBoneData(aiNode* node, int index, int parent);
	void ReadMeshData(aiNode* node, int parentBone);
	void WriteMeshData(wstring saveFolder, wstring fileName);

private:
	Assimp::Importer* importer;
	const aiScene* scene;

	wstring fbxFile;
	wstring saveFolder;
	wstring saveName;

	vector<struct AsMaterial *> materials;
	vector<struct AsBone*> bones;
	vector<struct AsMesh*> meshes;
};
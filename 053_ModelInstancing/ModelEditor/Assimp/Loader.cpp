#include "stdafx.h"
#include "Loader.h"
#include "Type.h"
#include "Utilities/Xml.h"
#include "Utilities/BinaryFile.h"

Loader::Loader(wstring file, wstring saveFolder, wstring saveName)
	: fbxFile(file), saveFolder(saveFolder), saveName(saveName)
{
	importer = new Assimp::Importer();

	scene = importer->ReadFile
	(
		String::ToString(fbxFile),
		aiProcess_ConvertToLeftHanded |
		aiProcess_Triangulate | aiProcess_GenNormals
	);
	assert(scene != NULL);
}

Loader::~Loader()
{
	SAFE_DELETE(importer);
}

void Loader::ExportMaterial(wstring saveFolder, wstring fileName)
{
	ReadMaterial();


	wstring tempFolder = saveFolder.size() < 1 ? this->saveFolder : saveFolder;
	wstring tempName = fileName.size() < 1 ? this->saveName : fileName;

	WriteMaterial(tempFolder, tempName + L".material");
}

void Loader::ExportMesh(wstring saveFolder, wstring fileName)
{
	ReadBoneData(scene->mRootNode, -1, -1);
	//ReadSkinData();


	wstring tempFolder = saveFolder.size() < 1 ? this->saveFolder : saveFolder;
	wstring tempName = fileName.size() < 1 ? this->saveName : fileName;

	WriteMeshData(tempFolder, tempName + L".mesh");
}

void Loader::ReadMaterial()
{
	UINT count = scene->mNumMaterials;

	for (UINT i = 0; i < count; i++)
	{
		aiMaterial* asMaterial = scene->mMaterials[i];

		AsMaterial* material = new AsMaterial();
		ZeroMemory(material, sizeof(AsMaterial));

		material->Name = asMaterial->GetName().C_Str();

		aiString file;
		aiColor3D color;

		asMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color);
		material->Ambient = D3DXCOLOR(color.r, color.g, color.b, 1.0f);

		asMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &file);
		material->DiffuseFile = file.C_Str();

		asMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		material->Diffuse = D3DXCOLOR(color.r, color.g, color.b, 1.0f);

		asMaterial->GetTexture(aiTextureType_SPECULAR, 0, &file);
		material->SpecularFile = file.C_Str();

		asMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color);
		material->Specular = D3DXCOLOR(color.r, color.g, color.b, 1.0f);


		float specular;

		asMaterial->Get(AI_MATKEY_SHININESS, specular);
		material->Specular.a = specular;

		asMaterial->Get(AI_MATKEY_SHININESS_STRENGTH, specular);
		material->SpecularExp = specular;


		asMaterial->GetTexture(aiTextureType_NORMALS, 0, &file);
		material->NormalFile = file.C_Str();

		materials.push_back(material);
	}
}

void Loader::WriteMaterial(wstring saveFolder, wstring fileName)
{
	Path::CreateFolders(saveFolder);

	Xml::XMLDocument* document = new Xml::XMLDocument();
	Xml::XMLDeclaration *decl = document->NewDeclaration();
	document->LinkEndChild(decl);

	Xml::XMLElement* root = document->NewElement("Materials");
	root->SetAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
	root->SetAttribute("xmlns:xsd", "http://www.w3.org/2001/XMLSchema");
	document->LinkEndChild(root);


	for (AsMaterial* material : materials)
	{
		Xml::XMLElement* node = document->NewElement("Material");
		root->LinkEndChild(node);


		Xml::XMLElement* element = NULL;

		element = document->NewElement("Name");
		element->SetText(material->Name.c_str());
		node->LinkEndChild(element);


		string save = String::ToString(saveFolder);

		element = document->NewElement("DiffuseFile");
		material->DiffuseFile = WriteTexture(material->DiffuseFile);
		element->SetText(material->DiffuseFile.c_str());
		node->LinkEndChild(element);

		element = document->NewElement("SpecularFile");
		material->SpecularFile = WriteTexture(material->SpecularFile);
		element->SetText(material->SpecularFile.c_str());
		node->LinkEndChild(element);

		element = document->NewElement("NormalFile");
		material->NormalFile = WriteTexture(material->NormalFile);
		element->SetText(material->NormalFile.c_str());
		node->LinkEndChild(element);


		element = document->NewElement("Ambient");
		node->LinkEndChild(element);
		WriteXmlColor(document, element, material->Ambient);

		element = document->NewElement("Diffuse");
		node->LinkEndChild(element);

		WriteXmlColor(document, element, material->Diffuse);

		element = document->NewElement("Specular");
		node->LinkEndChild(element);

		WriteXmlColor(document, element, material->Specular);

		element = document->NewElement("SpecularExp");
		element->SetText(material->SpecularExp);
		node->LinkEndChild(element);

		SAFE_DELETE(material);
	}

	string file = String::ToString(saveFolder + fileName);
	document->SaveFile(file.c_str());
}

void Loader::WriteXmlColor(Xml::XMLDocument * document, Xml::XMLElement * element, D3DXCOLOR & color)
{
	Xml::XMLElement* r = document->NewElement("R");
	r->SetText(color.r);
	element->LinkEndChild(r);

	Xml::XMLElement* g = document->NewElement("G");
	g->SetText(color.g);
	element->LinkEndChild(g);

	Xml::XMLElement* b = document->NewElement("B");
	b->SetText(color.b);
	element->LinkEndChild(b);

	Xml::XMLElement* a = document->NewElement("A");
	a->SetText(color.a);
	element->LinkEndChild(a);
}

string Loader::WriteTexture(string file)
{
	if (file.length() < 1)return "";

	const aiTexture* texture = scene->GetEmbeddedTexture(file.c_str());
	if (texture != NULL)
	{
		#pragma region 내장 텍스쳐
		string saveName = Path::GetFileNameWithoutExtension(file.c_str());
		saveName = String::ToString(saveFolder) + saveName + ".png";

		if (texture->mHeight < 1)
		{
			FILE* fp;
			fopen_s(&fp, saveName.c_str(), "wb");
			fwrite(texture->pcData, texture->mWidth, 1, fp);
			fclose(fp);

			return saveName;
		}
		else
		{
			ID3D11Texture2D* dest;
			D3D11_TEXTURE2D_DESC destDesc;
			ZeroMemory(&destDesc, sizeof(D3D11_TEXTURE2D_DESC));
			destDesc.Width = texture->mWidth;
			destDesc.Height = texture->mHeight;
			destDesc.MipLevels = 1;
			destDesc.ArraySize = 1;
			destDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			destDesc.SampleDesc.Count = 1;
			destDesc.SampleDesc.Quality = 0;

			D3D11_SUBRESOURCE_DATA subResource = { 0 };
			subResource.pSysMem = texture->pcData;


			HRESULT hr;
			hr = D3D::GetDevice()->CreateTexture2D(&destDesc, &subResource, &dest);
			assert(SUCCEEDED(hr));

			D3DX11SaveTextureToFileA(D3D::GetDC(), dest, D3DX11_IFF_PNG, saveName.c_str());

			return saveName;
		}
		#pragma endregion
	}
	else
	{
		string directory = Path::GetDirectoryName(String::ToString(fbxFile));
		string origin = directory + file;
		String::Replace(&origin, "\\", "/");// 역슬래쉬 변환

		bool bExist = Path::ExistFile(origin);
		if (bExist == false) 
			return "";

		string saveName = String::ToString(saveFolder) + Path::GetFileName(file);
		CopyFileA(origin.c_str(), saveName.c_str(), FALSE);

		return saveName;
	}

	return "";
}

void Loader::ReadBoneData(aiNode * node, int index, int parent)
{
	AsBone* bone = new AsBone();
	bone->Index = index;
	bone->Parent = parent;
	bone->Name = node->mName.C_Str();

	D3DXMATRIX transform(node->mTransformation[0]);
	D3DXMatrixTranspose(&bone->Transform, &transform);

	D3DXMATRIX temp;
	if (parent == -1)
		D3DXMatrixIdentity(&temp);
	else
		temp = bones[parent]->Transform;

	bone->Transform = bone->Transform * temp;
	bones.push_back(bone);

	ReadMeshData(node, index);

	for (int i = 0; i < node->mNumChildren; i++)
		ReadBoneData(node->mChildren[i], bones.size(), index);
}

void Loader::ReadMeshData(aiNode * node, int parentBone)
{
	if (node->mNumMeshes < 1)
		return;

	AsMesh* asMesh = new AsMesh();
	asMesh->Name = node->mName.C_Str();
	asMesh->ParentBone = parentBone;

	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		UINT index = node->mMeshes[i];
		aiMesh* mesh = scene->mMeshes[index];

		UINT startVertex = asMesh->Vertices.size();
		UINT startIndex = asMesh->Indices.size();
		
		for (UINT m = 0; m < mesh->mNumVertices; m++)
		{
			VertexTextureNormalBlend vertex;

			memcpy(&vertex.Position, &mesh->mVertices[m], sizeof(D3DXVECTOR3));

			if (mesh->HasTextureCoords(0) == true)
				memcpy(&vertex.Uv, &mesh->mTextureCoords[0][m], sizeof(D3DXVECTOR2));

			memcpy(&vertex.Normal, &mesh->mNormals[m], sizeof(D3DXVECTOR3));

			asMesh->Vertices.push_back(vertex);
		}//for(m)

		for (UINT f = 0; f < mesh->mNumFaces; f++)
		{
			aiFace& face = mesh->mFaces[f];

			for (UINT k = 0; k < face.mNumIndices; k++)
			{
				asMesh->Indices.push_back(face.mIndices[k]);
				asMesh->Indices.back() += startVertex;
			}//for(k)
		}//for(f)


		AsMeshPart* meshPart = new AsMeshPart();

		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		meshPart->Name = mesh->mName.C_Str();
		meshPart->MaterialName = material->GetName().C_Str();
		meshPart->StartVertex = startVertex;
		meshPart->VertexCount = mesh->mNumVertices;
		meshPart->StartIndex = startIndex;
		meshPart->IndexCount = mesh->mNumFaces* mesh->mFaces->mNumIndices;

		asMesh->MeshParts.push_back(meshPart);
	}//for(i)

	meshes.push_back(asMesh);

	int a = 10;
}

void Loader::WriteMeshData(wstring saveFolder, wstring fileName)
{
	Path::CreateFolders(saveFolder);

	BinaryWriter* w = new BinaryWriter();
	w->Open(saveFolder + fileName);

	w->UInt(bones.size());
	for (AsBone* bone : bones)
	{
		w->Int(bone->Index);
		w->String(bone->Name);

		w->Int(bone->Parent);

		w->Matrix(bone->Transform);

		SAFE_DELETE(bone);
	}


	w->UInt(meshes.size());
	for (AsMesh* meshData : meshes)
	{
		w->String(meshData->Name);
		w->Int(meshData->ParentBone);

		w->UInt(meshData->Vertices.size());
		w->Byte(&meshData->Vertices[0], sizeof(VertexTextureNormalBlend) * meshData->Vertices.size());

		w->UInt(meshData->Indices.size());
		w->Byte(&meshData->Indices[0], sizeof(UINT) * meshData->Indices.size());

		w->UInt(meshData->MeshParts.size());
		for (AsMeshPart* part : meshData->MeshParts)
		{
			w->String(part->Name);
			w->String(part->MaterialName);

			w->UInt(part->StartVertex);
			w->UInt(part->VertexCount);

			w->UInt(part->StartIndex);
			w->UInt(part->IndexCount);

			SAFE_DELETE(part);
		}

		SAFE_DELETE(meshData);
	}

	w->Close();
	SAFE_DELETE(w);
}

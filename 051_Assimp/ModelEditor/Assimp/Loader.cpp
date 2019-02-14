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
		aiProcess_Triangulate | aiProcess_OptimizeMeshes
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
	WriteTexture();

	int a = 10;
}

void Loader::ReadMaterial()
{
	UINT count = scene->mNumMaterials;

	for (UINT i = 0; i < count; i++)
	{
		aiMaterial* asMaterial = scene->mMaterials[i];

		AssimpMaterial* material = new AssimpMaterial();
		ZeroMemory(material, sizeof(AssimpMaterial));

		material->Name = asMaterial->GetName().C_Str();

		aiString file;
		aiColor3D color;
		aiReturn result;

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


	for (AssimpMaterial* material : materials)
	{
		Xml::XMLElement* node = document->NewElement("Material");
		root->LinkEndChild(node);


		Xml::XMLElement* element = NULL;

		element = document->NewElement("Name");
		element->SetText(material->Name.c_str());
		node->LinkEndChild(element);


		string save = String::ToString(saveFolder);

		element = document->NewElement("DiffuseFile");
		material->DiffuseFile = Path::GetFileName(material->DiffuseFile);
		material->DiffuseFile = material->DiffuseFile.length < 1 ? "" : save + material->DiffuseFile;
		element->SetText(material->DiffuseFile.c_str());
		node->LinkEndChild(element);

		element = document->NewElement("SpecularFile");
		material->SpecularFile = Path::GetFileName(material->SpecularFile);
		material->SpecularFile = material->SpecularFile.length < 1 ? "" : save + material->SpecularFile;
		element->SetText(material->SpecularFile.c_str());
		node->LinkEndChild(element);

		element = document->NewElement("NormalFile");
		material->NormalFile = Path::GetFileName(material->NormalFile);
		material->NormalFile = material->NormalFile.length < 1 ? "" : save + material->NormalFile;
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

void Loader::WriteTexture()
{
	for (UINT i = 0; i < scene->mNumTextures; i++)
	{
		aiTexture* origin = scene->mTextures[i];
		if (origin == NULL) continue;


		const aiTexture* texture = scene->GetEmbeddedTexture(origin->mFilename.C_Str());
		if (texture != NULL)
		{
			#pragma region 내장 텍스쳐
			string file = Path::GetFileNameWithoutExtension(origin->mFilename.C_Str());
			file = String::ToString(saveFolder) + file + ".png";


			if (texture->mHeight < 1) //압축된 텍스쳐
			{
				FILE* p = fopen(file.c_str(), "wb");
				fwrite(texture->pcData, texture->mWidth, 1, p);
				fclose(p);
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

				D3DX11SaveTextureToFileA(D3D::GetDC(), dest, D3DX11_IFF_PNG, file.c_str());
			}// if(texture->mHeight)
			#pragma endregion
		}
		else
		{
			#pragma region 외장 텍스쳐
			#pragma endregion
		} //if(texture)
	}

	int a = 0;
}
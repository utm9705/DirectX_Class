#include "framework.h"
#include "Model.h"
#include "ModelBone.h"
#include "ModelMesh.h"
#include "ModelMeshPart.h"
#include "../Utilities/Xml.h"
#include "../Utilities/BinaryFile.h"


void Model::ReadMaterial(wstring file)
{
	file = L"../../_Textures/" + file + L".material";

	Models::LoadMaterial(file, &materials);
}

void Model::ReadMesh(wstring file)
{
	file = L"../../_Models/" + file + L".mesh";

	Models::LoadMesh(file, &bones, &meshes);

	BindingBone();
	BindingMesh();
}

void Model::BindingBone()
{
	this->root = bones[0];
	for (ModelBone* bone : bones)
	{
		if (bone->parentIndex > -1)
		{
			bone->parent = bones[bone->parentIndex];
			bone->parent->childs.push_back(bone);
		}
		else
			bone->parent = NULL;
	}
}

void Model::BindingMesh()
{
	for (ModelMesh* mesh : meshes)
	{
		for (ModelBone* bone : bones)
		{
			if (mesh->parentBoneIndex == bone->index)
			{
				mesh->parentBone = bone;

				break;
			}
		}

		for (ModelMeshPart* part : mesh->meshParts)
		{
			for (Material* material : materials)
			{
				if (part->materialName == material->Name())
				{
					part->material = material;

					break;
				}
			}
		}

		mesh->Binding();
	}
}

///////////////////////////////////////////////////////////////////////////////

map<wstring, vector<Material *>> Models::materialMap;
void Models::LoadMaterial(wstring file, vector<Material*>* materials)
{
	if (materialMap.count(file) < 1)
		ReadMaterialData(file);

	for (Material* material : materialMap[file])
	{
		Material* temp = new Material();
		temp->Name(material->Name());

		temp->Ambient(material->Ambient());
		temp->Diffuse(material->Diffuse());
		temp->Specular(material->Specular());
		temp->Shininess(material->Shininess());

		if (material->DiffuseMap() != NULL)
			temp->DiffuseMap(material->DiffuseMap()->GetFile());

		if (material->SpecularMap() != NULL)
			temp->SpecularMap(material->SpecularMap()->GetFile());

		if (material->NormalMap() != NULL)
			temp->NormalMap(material->NormalMap()->GetFile());

		materials->push_back(temp);
	}
}

void Models::ReadMaterialData(wstring file)
{
	vector<Material *> materials;

	Xml::XMLDocument* document = new Xml::XMLDocument();

	wstring tempFile = file;
	Xml::XMLError error = document->LoadFile(String::ToString(tempFile).c_str());
	assert(error == Xml::XML_SUCCESS);

	Xml::XMLElement* root = document->FirstChildElement();
	Xml::XMLElement* matNode = root->FirstChildElement();


	do
	{
		Xml::XMLElement* node = NULL;

		Material* material = new Material();

		node = matNode->FirstChildElement();
		material->Name(String::ToWString(node->GetText()));


		wstring directory = Path::GetDirectoryName(tempFile);
		String::Replace(&directory, L"../../_Textures/", L"");

		node = node->NextSiblingElement();
		wstring diffuseTexture = String::ToWString(node->GetText());
		if (diffuseTexture.length() > 0)
			material->DiffuseMap(directory + diffuseTexture);

		node = node->NextSiblingElement();
		wstring specularTexture = String::ToWString(node->GetText());
		if (specularTexture.length() > 0)
			material->SpecularMap(directory + specularTexture);

		node = node->NextSiblingElement();
		wstring normalTexture = String::ToWString(node->GetText());
		if (normalTexture.length() > 0)
			material->NormalMap(directory + normalTexture);


		D3DXCOLOR color;

		//Ambient Color
		node = node->NextSiblingElement();
		color.r = node->FloatAttribute("R");
		color.g = node->FloatAttribute("G");
		color.b = node->FloatAttribute("B");
		color.a = node->FloatAttribute("A");
		material->Ambient(color);

		//Diffuse Color
		node = node->NextSiblingElement();
		color.r = node->FloatAttribute("R");
		color.g = node->FloatAttribute("G");
		color.b = node->FloatAttribute("B");
		color.a = node->FloatAttribute("A");
		material->Diffuse(color);


		//Specular Color
		node = node->NextSiblingElement();
		color.r = node->FloatAttribute("R");
		color.g = node->FloatAttribute("G");
		color.b = node->FloatAttribute("B");
		color.a = node->FloatAttribute("A");
		material->Specular(color);


		node = node->NextSiblingElement();
		material->Shininess(node->FloatText());

		materials.push_back(material);

		matNode = matNode->NextSiblingElement();
	} while (matNode != NULL);

	SAFE_DELETE(document);

	materialMap[file] = materials;
}

///////////////////////////////////////////////////////////////////////////////
map<wstring, Models::MeshData> Models::meshDataMap;
void Models::LoadMesh(wstring file, vector<ModelBone*>* bones, vector<ModelMesh*>* meshes)
{
	if (meshDataMap.count(file) < 1)
		ReadMeshData(file);


	MeshData data = meshDataMap[file];
	for (ModelBone* bone : data.Bones)
	{
		ModelBone* temp = NULL;
		bone->Copy(&temp);

		bones->push_back(temp);
	}

	for (ModelMesh* mesh : data.Meshes)
	{
		ModelMesh* temp = NULL;
		mesh->Copy(&temp);

		meshes->push_back(temp);
	}
}

void Models::ReadMeshData(wstring file)
{
	BinaryReader* r = new BinaryReader();
	r->Open(file);

	vector<ModelBone *> bones;
	vector<ModelMesh *> meshes;


	UINT count = 0;

	count = r->UInt();
	for (UINT i = 0; i < count; i++)
	{
		ModelBone* bone = new ModelBone();

		bone->index = r->Int();
		bone->name = String::ToWString(r->String());
		bone->parentIndex = r->Int();
		bone->transform = r->Matrix();

		bones.push_back(bone);
	}

	count = r->UInt();
	for (UINT i = 0; i < count; i++)
	{
		ModelMesh* mesh = new ModelMesh();

		mesh->name = String::ToWString(r->String());
		mesh->parentBoneIndex = r->Int();

		//VertexData
		{
			UINT count = r->UInt();

			vector<VertexTextureNormalBlend> vertices;
			vertices.assign(count, VertexTextureNormalBlend());

			void* ptr = (void *)&(vertices[0]);
			r->Byte(&ptr, sizeof(VertexTextureNormalBlend) * count);


			mesh->vertices = new VertexTextureNormalBlend[count];
			mesh->vertexCount = count;
			copy
			(
				vertices.begin(), vertices.end(),
				stdext::checked_array_iterator<VertexTextureNormalBlend *>(mesh->vertices, count)
			);
		}

		//IndexData
		{
			UINT count = r->UInt();

			vector<UINT> indices;
			indices.assign(count, UINT());

			void* ptr = (void *)&(indices[0]);
			r->Byte(&ptr, sizeof(UINT) * count);


			mesh->indices = new UINT[count];
			mesh->indexCount = count;
			copy
			(
				indices.begin(), indices.end(),
				stdext::checked_array_iterator<UINT *>(mesh->indices, count)
			);
		}


		UINT partCount = r->UInt();
		for (UINT k = 0; k < partCount; k++)
		{
			ModelMeshPart* meshPart = new ModelMeshPart();
			meshPart->parent = mesh;
			
			meshPart->name = String::ToWString(r->String());
			meshPart->materialName = String::ToWString(r->String());

			meshPart->startVertex = r->UInt();
			meshPart->vertexCount = r->UInt();

			meshPart->startIndex = r->UInt();
			meshPart->indexCount = r->UInt();

			mesh->meshParts.push_back(meshPart);
		}//for(k)

		meshes.push_back(mesh);
	}//for(i)

	r->Close();
	SAFE_DELETE(r);


	MeshData data;
	data.Bones.assign(bones.begin(), bones.end());
	data.Meshes.assign(meshes.begin(), meshes.end());

	meshDataMap[file] = data;
}

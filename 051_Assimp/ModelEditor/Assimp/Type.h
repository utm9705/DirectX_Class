#pragma once
#include "stdafx.h"

struct AsMaterial
{
	string Name;

	D3DXCOLOR Ambient;

	string DiffuseFile;
	D3DXCOLOR Diffuse;

	string SpecularFile;
	D3DXCOLOR Specular;
	float SpecularExp;

	string NormalFile;
};

struct AsBone
{
	int Index;
	string Name;

	int Parent;

	D3DXMATRIX Transform;
};

struct AsMeshPart
{
	string Name;
	string MaterialName;

	UINT StartVertex;
	UINT VertexCount;

	UINT StartIndex;
	UINT IndexCount;
};

struct AsMesh
{
	string Name;
	int ParentBone;

	aiMesh* Mesh;

	vector<VertexTextureNormalBlend> Vertices;
	vector<UINT> Indices;

	vector<AsMeshPart *> MeshParts;
};
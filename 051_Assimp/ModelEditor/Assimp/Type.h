#pragma once
#include "stdafx.h"

struct AssimpMaterial
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
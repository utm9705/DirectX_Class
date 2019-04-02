#include "Framework.h"
#include "Material.h"

Material::Material()
{
	Initialize();
}

Material::Material(Shader * shader)
{
	Initialize();

	SetShader(shader);
}

void Material::Initialize()
{
	name = L"";

	diffuseMap = NULL;
	specularMap = NULL;
	normalMap = NULL;

	buffer = new ConstantBuffer(&colorDesc, sizeof(ColorDesc));
}

Material::~Material()
{
	SAFE_DELETE(diffuseMap);
	SAFE_DELETE(specularMap);
	SAFE_DELETE(normalMap);

	SAFE_DELETE(buffer);
}

void Material::SetShader(Shader * shader)
{
	sBuffer = shader->AsConstantBuffer("CB_Material");

	sDiffuseMap = shader->AsSRV("DiffuseMap");
	sSpecularMap = shader->AsSRV("SpecularMap");
	sNormalMap = shader->AsSRV("NormalMap");
}

void Material::Ambient(D3DXCOLOR & color)
{
	colorDesc.Ambient = color;
}

void Material::Ambient(float r, float g, float b, float a)
{
	Ambient(D3DXCOLOR(r, g, b, a));
}

void Material::Diffuse(D3DXCOLOR & color)
{
	colorDesc.Diffuse = color;
}

void Material::Diffuse(float r, float g, float b, float a)
{
	Diffuse(D3DXCOLOR(r, g, b, a));
}

void Material::Specular(D3DXCOLOR & color)
{
	colorDesc.Specular = color;
}

void Material::Specular(float r, float g, float b, float a)
{
	Specular(D3DXCOLOR(r, g, b, a));
}

void Material::DiffuseMap(string file)
{
	DiffuseMap(String::ToWString(file));
}

void Material::DiffuseMap(wstring file)
{
	SAFE_DELETE(diffuseMap);

	diffuseMap = new Texture(file);
}

void Material::DIffuseMapNull()
{
	SAFE_DELETE(diffuseMap);

	diffuseMap = NULL;
}

void Material::SpecularMap(string file)
{
	SpecularMap(String::ToWString(file));
}

void Material::SpecularMap(wstring file)
{
	SAFE_DELETE(specularMap);

	specularMap = new Texture(file);
}

void Material::SpecularMapNull()
{
	SAFE_DELETE(specularMap);

	specularMap = NULL;
}

void Material::NormalMap(string file)
{
	NormalMap(String::ToWString(file));
}

void Material::NormalMap(wstring file)
{
	SAFE_DELETE(normalMap);

	normalMap = new Texture(file);
}

void Material::NormalMapNull()
{
	SAFE_DELETE(normalMap);

	normalMap = NULL;
}

void Material::Render()
{
	buffer->Apply();
	sBuffer->SetConstantBuffer(buffer->Buffer());

	if (diffuseMap != NULL)
		sDiffuseMap->SetResource(diffuseMap->SRV());
	else
		sDiffuseMap->SetResource(NULL);

	if (specularMap != NULL)
		sSpecularMap->SetResource(specularMap->SRV());
	else
		sSpecularMap->SetResource(NULL);

	if (normalMap != NULL)
		sNormalMap->SetResource(normalMap->SRV());
	else
		sNormalMap->SetResource(NULL);
}
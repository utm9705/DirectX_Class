#pragma once

class Mesh : public Renderer
{
public:
	typedef VertexTextureNormalTangent MeshVertex;

public:
	Mesh(Shader* shader);
	virtual ~Mesh();

	void DiffuseMap(wstring file);
	void SpecularMap(wstring file);
	void NormalMap(wstring file);

	void Render();

protected:
	virtual void Create() = 0;

protected:
	MeshVertex* vertices;
	UINT* indices;

	UINT vertexCount;
	UINT indexCount;

	Texture* diffuseMap;
	ID3DX11EffectShaderResourceVariable* sDiffuseMap;

	Texture* normalMap;
	ID3DX11EffectShaderResourceVariable* sNormalMap;

	Texture* specularMap;
	ID3DX11EffectShaderResourceVariable* sSpecularMap;
};
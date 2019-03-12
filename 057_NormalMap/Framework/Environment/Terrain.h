#pragma once

class Terrain : public Renderer
{
public:
	Terrain(Shader* shader, wstring heightMap);
	~Terrain();

	void BaseTexture(wstring file);

	void Ready();
	void Update();
	void Render();

	float GetHeight(float positionX, float positionZ);

private:
	void CreateData();
	void CreateNormalData();

private:
	wstring heightMapFile;
	Texture* heightMap;

	UINT width, height;

	VertexTextureNormal* vertices;
	UINT* indices;
	UINT vertexCount, indexCount;

	Texture* baseTexture;
	ID3DX11EffectShaderResourceVariable* sBaseTexture;
};
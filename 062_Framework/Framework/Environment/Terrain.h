#pragma once

class Terrain : public Renderer
{
public:
	struct InitializeInfo;

public:
	Terrain(InitializeInfo& info);
	~Terrain();

	void BaseTexture(wstring file);
	void LayerTexture(wstring layer, wstring alpha);

	void Ready();
	void Update();
	void Render();

private:
	bool InBounds(UINT row, UINT col);
	void CalcBoundY();
	void CalcPatchBounds(UINT row, UINT col);

	void CreateVertexData();
	void CreateIndexData();

public:
	struct InitializeInfo
	{
		Shader* shader;

		wstring heightMap;
		float CellSpacing = 1.0f;
		UINT CellsPerPatch = 64;
		float HeightRatio = 20;
	};

private:
	struct VertexTerrain
	{
		D3DXVECTOR3 Position;
		D3DXVECTOR2 Uv;
		D3DXVECTOR2 BoundsY;
	};

	struct BufferDesc
	{
		float MinDistance = 0.0f;
		float MaxDistance = 1000.0f;
		float MinTessellation = 1.0f;
		float MaxTessellation = 4.0f;

		float TexelCellSpaceU;
		float TexelCellSpaceV;
		float WorldCellSpace = 1.0f;
		float HeightRatio = 10.0f;

		D3DXVECTOR2 TexScale = D3DXVECTOR2(1, 1);
		float Padding2[2];

		D3DXPLANE WorldFrustumPlanes[6];
	} bufferDesc;

private:
	UINT faceCount;
	UINT patchVertexRows;
	UINT patchVertexCols;

private:
	InitializeInfo info;

	ConstantBuffer* buffer;
	ID3DX11EffectConstantBuffer* sBuffer;

	class Frustum* frustum;
	Texture* heightMap;
	vector<D3DXCOLOR> heightMapPixel;

	UINT width, height;

	VertexTerrain* vertices;
	UINT* indices;

	vector<D3DXVECTOR2> bounds;

	Texture* baseTexture;
	ID3DX11EffectShaderResourceVariable* sBaseTexture;

	Texture* layerTexture;
	ID3DX11EffectShaderResourceVariable* sLayerTexture;

	Texture* alphaTexture;
	ID3DX11EffectShaderResourceVariable* sAlphaTexture;

	ID3DX11EffectShaderResourceVariable* sHeightTexture;
};
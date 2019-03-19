#pragma once

#define MAX_COUNT 10000

class SnowInst : public Renderer
{
public:
	struct InitDesc;

public:
	SnowInst(InitDesc& initDesc);
	~SnowInst();

	void Update() override;
	void Render() override;

private:
	void CreateData();

public:
	struct InitDesc
	{
		D3DXVECTOR3 Size;
		UINT Count;
		UINT InstCount;
	} initDesc;

private:
	struct BufferDesc
	{
		D3DXCOLOR Diffuse = D3DXCOLOR(1, 1, 1, 1);

		D3DXVECTOR3 Velocity = D3DXVECTOR3(-1, -5, 0);
		float DrawDistance = 1000.0f;

		D3DXVECTOR3 Origin = D3DXVECTOR3(0, 0, 0);
		float Turbulence = 5;

		D3DXVECTOR3 Size = D3DXVECTOR3(0, 0, 0);
		float Padding2;
	} bufferDesc;


private:

	Shader* shader;

	VertexTextureNormal* vertices;
	UINT * indices;

	UINT vertexCount;
	UINT indexCount;
	UINT Count;

	ID3D11Buffer* instanceBuffer;
	D3DXMATRIX world[MAX_COUNT];
	UINT instCount;

	TextureArray* textures;
	///
	D3DXVECTOR4 Pos[1000];

	//

	Texture* texture;

	CBuffer* buffer;
	ID3DX11EffectConstantBuffer* sBuffer;
};
#pragma once

class Snow : public Renderer
{
public:
	struct InitDesc;

public:
	Snow(InitDesc& desc);
	~Snow();

	void Update() override;
	void Render() override;

private:
	void CreateData();

public:
	struct InitDesc
	{
		D3DXVECTOR3 Size;
		UINT Count;
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

	struct VertexSnow
	{
		D3DXVECTOR3 Position;
		D3DXVECTOR2 Uv;
		D3DXVECTOR2 Random;
		float Scale;
	};

private:
	Shader* shader;
	CBuffer* buffer;
	ID3DX11EffectConstantBuffer* sBuffer;

	VertexSnow* vertices;
	UINT* indices;

	Texture* texture;
};
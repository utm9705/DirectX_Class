#pragma once

class Rain : public Renderer
{
public:
	struct InitDesc;

public:
	Rain(InitDesc& desc);
	~Rain();

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

		D3DXVECTOR3 Velocity = D3DXVECTOR3(-10, -100, 0);
		float DrawDistance = 1000.0f;

		D3DXVECTOR3 Origin = D3DXVECTOR3(0, 0, 0);
		float Padding;

		D3DXVECTOR3 Size = D3DXVECTOR3(0, 0, 0);
		float Padding2;
	} bufferDesc;

	struct VertexRain
	{
		D3DXVECTOR3 Position;
		D3DXVECTOR2 Uv;
		D3DXVECTOR2 Scale;
	};

private:
	Shader* shader;
	CBuffer* buffer;
	ID3DX11EffectConstantBuffer* sBuffer;

	VertexRain* vertices;
	UINT* indices;

	Texture* texture;
};
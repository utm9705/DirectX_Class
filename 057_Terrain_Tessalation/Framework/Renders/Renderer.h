#pragma once

class Renderer
{
public:
	Renderer();
	Renderer(Shader* shader);
	virtual ~Renderer();

	Shader* GetShader() { return shader; }
	virtual void SetShader(Shader* shader);

	void Pass(UINT val) { pass = val; }
	UINT Pass() { return pass; }

	void Position(float x, float y, float z);
	void Position(D3DXVECTOR3& vec);
	void Position(D3DXVECTOR3* vec);

	void Scale(float x, float y, float z);
	void Scale(D3DXVECTOR3& vec);
	void Scale(D3DXVECTOR3* vec);

	void Rotation(float x, float y, float z);
	void Rotation(D3DXVECTOR3& vec);
	void Rotation(D3DXVECTOR3* vec);

	void RotationDegree(float x, float y, float z);
	void RotationDegree(D3DXVECTOR3& vec);
	void RotationDegree(D3DXVECTOR3* vec);

	void Direction(D3DXVECTOR3* vec);
	void Up(D3DXVECTOR3* vec);
	void Right(D3DXVECTOR3* vec);


	virtual void Update();
	virtual void Render();

	D3DXMATRIX& World() { return frameDesc.World; }

protected:
	virtual void UpdateWorld();

protected:
	void CreateVertexBuffer(void* pData, UINT count, UINT stride, UINT offset);
	void CreateIndexBuffer(UINT* pData, UINT count);

	void Topology(D3D11_PRIMITIVE_TOPOLOGY val) { topology = val; }

private:
	void Initialize();

protected:
	struct FrameDesc
	{
		D3DXMATRIX World;
		D3DXMATRIX View;
		D3DXMATRIX ViewInverse;
		D3DXMATRIX Projection;
		D3DXMATRIX Vp;

		D3DXCOLOR LightColor;
		D3DXVECTOR3 LightDirection;
		D3DXVECTOR3 LightPosition;

		float Time;

		float Padding;
	} frameDesc;

protected:
	Shader* shader;
	UINT pass;

	UINT stride, offset;
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

private:
	D3D11_PRIMITIVE_TOPOLOGY topology;

	CBuffer* frameBuffer;
	ID3DX11EffectConstantBuffer* sFrameDesc;

	D3DXVECTOR3 position;
	D3DXVECTOR3 scale;
	D3DXVECTOR3 rotation;

	D3DXVECTOR3 direction;
	D3DXVECTOR3 up;
	D3DXVECTOR3 right;
};
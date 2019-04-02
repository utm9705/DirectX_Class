#pragma once

class SkyDome
{
public:
	SkyDome(Shader* shader);
	~SkyDome();

	void Ready();
	void Update();
	void Render();

private:
	void CreateDomeData();

private:
	Shader* shader;

	UINT domeCount;

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	UINT vertexCount, indexCount;
};
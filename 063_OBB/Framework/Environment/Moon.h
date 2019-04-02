#pragma once

class Moon 
{
public:
	Moon(Shader* shader);
	~Moon();

	void Ready();
	void Update();

	void Render();

	float GetMoonAlpha(float theta);

	D3DXMATRIX GetTransform(float theta);
	D3DXMATRIX GetGlowTransform(float theta);

private:
	Shader* shader;

	ID3D11Buffer * vertexBuffer;

	float distance, glowDistance;
};
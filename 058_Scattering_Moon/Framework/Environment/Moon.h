#pragma once

class Moon
{
public:
	Moon(Shader* shader);
	~Moon();

	void Ready();
	void Update();

	void Render();

private:

private:
	Shader* shader;

	ID3D11Buffer * vertexBuffer;
};
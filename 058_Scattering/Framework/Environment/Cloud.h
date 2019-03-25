#pragma once

class Cloud
{
public:
	Cloud(Shader* shader);
	~Cloud();

	ID3D11ShaderResourceView* SRV() { return srv; }

	void Ready();
	void Update();
	void Render();

private:
	void CreateTexture();

private:
	Shader* shader;

	ID3D11Buffer* vertexBuffer;

	ID3D11Texture2D* texture;
	ID3D11ShaderResourceView* srv;
};
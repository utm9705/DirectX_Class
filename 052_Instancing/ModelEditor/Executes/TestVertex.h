#pragma once
#include "Systems/IExecute.h"

class TestVertex : public IExecute
{
public:
	void Initialize() override;
	void Ready() override;
	void Destroy() override;

	void Update() override;

	void PreRender() override;

	void Render() override;
	void PostRender() override {}

	void ResizeScreen() override {}

private:
	Shader* shader;

	VertexTexture vertices[4];
	ID3D11Buffer* vertexBuffer[2];

	ID3D11Buffer* indexBuffer;

	D3DXMATRIX world[1000];

	TextureArray* textures;
};
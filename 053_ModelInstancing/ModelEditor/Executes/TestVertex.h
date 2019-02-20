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

	Vertex vertices[6];
	ID3D11Buffer* vertexBuffer[2];

	D3DXCOLOR color[2];
	D3DXMATRIX world[2][3];

	ID3D11Texture2D* worldData;
	ID3D11ShaderResourceView* worldSrv;
};
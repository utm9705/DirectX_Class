#pragma once

#define MAX_INST_MODEL 100

class ModelInstance
{
public:
	ModelInstance(class Model* model, wstring shaderFile);
	ModelInstance();

	void Ready();

	UINT AddWorld(D3DMATRIX& world);

	void Render();

private:
	void UpdateTransforms(UINT count);

private:
	class Model* model;

	UINT maxCount;
	D3DXMATRIX transforms[MAX_INST_MODEL][128];//본의 행렬

	ID3D11Texture2D* texture;
	ID3D11ShaderResourceView* srv;

	D3DXMATRIX worlds[MAX_INST_MODEL];
};
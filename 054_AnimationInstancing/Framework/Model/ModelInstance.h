#pragma once

#define MAX_INST_MODEL 100

class ModelInstance
{
public:
	ModelInstance(class Model* model, wstring shaderFile);
	virtual ~ModelInstance();

	void Ready();
	virtual void Render();

	UINT AddWorld(D3DMATRIX& world);

protected:
	virtual void MappedData();

protected:
	class Model* model;

	UINT maxCount;
	D3DXMATRIX** transforms;//본의 행렬

	ID3D11Texture2D* texture;
	ID3D11ShaderResourceView* srv;

	D3DXMATRIX worlds[MAX_INST_MODEL];

	UINT height;
};
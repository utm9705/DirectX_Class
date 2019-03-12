#pragma once

#define MAX_INST_MODEL 100

class ModelInstance
{
public:
	ModelInstance(class Model* model, Shader* shader);
	virtual ~ModelInstance();

	void Ready();	
	void Update();
	virtual void Render();

	UINT AddWorld(D3DXMATRIX& world);

protected:
	virtual void MappedData();

protected:
	class Model* model;

	UINT maxCount;
	D3DXMATRIX** transforms;

	ID3D11Texture2D* texture;
	ID3D11ShaderResourceView* srv;

	D3DXMATRIX worlds[MAX_INST_MODEL];

	UINT height;
};
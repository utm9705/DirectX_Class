#pragma once
#define MAX_MODEL_INSTANCE 100

class ModelRender
{
public:
	ModelRender(Shader* shader, Model* model);
	~ModelRender();

	void Update();
	void Render();

	UINT AddTransform();
	Transform* GetTransform(UINT index);
	void UpdateTransforms();

private:
	void UpdateBoneTransforms();

private:
	Model* model;

	UINT drawCount;

	D3DXMATRIX worlds[MAX_MODEL_INSTANCE];
	Transform* transforms[MAX_MODEL_INSTANCE];

	D3DXMATRIX boneTransforms[MAX_MODEL_INSTANCE][MAX_MODEL_TRANFORMS];
	ID3D11Texture2D* texture;
	ID3D11ShaderResourceView* srv;

	VertexBuffer* instanceBuffer;
};
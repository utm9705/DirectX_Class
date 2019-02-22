#pragma once

#define MAX_INST_MODEL 100

class ModelAnimInstance
{
public:
	ModelAnimInstance(Model* model, ModelClip* clip,wstring shaderFile);
	ModelAnimInstance();

	void Ready();

	UINT AddWorld(D3DMATRIX& world);

	void Render();

private:
	class Model* model;

	UINT maxCount;
	D3DXMATRIX** transforms;

	ID3D11Texture2D* texture;
	ID3D11ShaderResourceView* srv;

	ID3D11Texture2D* animTexture;
	ID3D11ShaderResourceView* animSrv;

	D3DXMATRIX worlds[MAX_INST_MODEL];
	class ModelClip* clip;

	struct FrameDesc
	{
		UINT Curr;
		UINT Next;
		float Time;
		float FrameTime;
	};
	FrameDesc frames[MAX_INST_MODEL];
};